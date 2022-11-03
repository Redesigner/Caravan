// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h" 

#include "CaravanAbility/Character/CharacterBase.h"
#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"

const float RotationSpeed = 180.0f;

AEnemyController::AEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyController::Tick(float DeltaTime)
{
	CurrentTarget = GetClosestTarget();
	SetFocus(CurrentTarget.Get());
	bool bIsClose = (MoveToActor(CurrentTarget.Get(), MinAttackDistance) == EPathFollowingRequestResult::AlreadyAtGoal );
	UpdateControlRotation(DeltaTime, true);

	if (bIsClose && Character.IsValid())
	{
		// We are technically overriding the built-in rotation,
		// so we need to make sure that the root motion is not running
		if (!Character->GetMesh()->IsPlayingRootMotion() && MovementComponent->MovementMode == EMovementMode::MOVE_Walking)
		{
			const FRotator DesiredRotation = FRotator(0.0f, ControlRotation.Yaw, 0.0f);
			const FRotator InterpRotation = FMath::RInterpConstantTo(Character->GetActorRotation(), DesiredRotation, DeltaTime, RotationSpeed);
			Character->SetActorRotation(InterpRotation);
		}
		TryExecuteAnyAttack();
	}
}


void AEnemyController::BeginPlay()
{
	// Populate our target lists once.
	PotentialTargets = GetTargets();
	GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &AEnemyController::TargetSpawned) );
	Super::BeginPlay();
}

void AEnemyController::OnPossess(APawn* PossessedPawn)
{
	// Update our weak references automatically, so we don't have to keep casting
	if (ACharacterBase* PawnAsCharacter = Cast<ACharacterBase>(PossessedPawn))
	{
		Character = PawnAsCharacter;

		if (UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent()))
		{
			MovementComponent = CharacterMovementComponent;
		}
		if (UCharacterAbilitySystemComponent* CharacterAscIn = Cast<UCharacterAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
		{
			CharacterAsc = CharacterAscIn;
			CharacterAsc->OnComboStateChanged.AddUObject(this, &AEnemyController::HandleComboStateChanged);
		}
	}
	Super::OnPossess(PossessedPawn);
}


void AEnemyController::TargetSpawned(AActor* Target)
{
	if (IsTarget(Target))
	{
		PotentialTargets.Add(Target);
	}
}


void AEnemyController::EndInternalCooldown()
{
	bInternalCooldown = false;
}

void AEnemyController::TryExecuteAnyAttack()
{
	if (bInternalCooldown)
	{
		// return early so we don't try to calculate the distance and angle every tick
		// this shouldn't be *too* expensive, but there really isn't a reason to calculate
		// that often
		return;
	}
	FVector DistanceToTarget = CurrentTarget->GetActorLocation() - Character->GetActorLocation();
	DistanceToTarget.Normalize();
	float AngleBetween = FMath::RadiansToDegrees(FMath::Acos(DistanceToTarget.Dot(Character->GetActorForwardVector())));
	if (AngleBetween <= AcceptableConeAngle)
	{
		// Attempt to execute an attack
		if (Character->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ValidAttackTagContainer))
		{
			// Set our internal cooldown, so the enemy isn't just spamming attacks as fast as a real player could
			bInternalCooldown = true;
			GetWorld()->GetTimerManager().SetTimer(InternalAttackCooldown,
				FTimerDelegate::CreateUObject(this, &AEnemyController::EndInternalCooldown),
				SuccessfulAttackCooldown, false);
		}
	}
}

void AEnemyController::HandleComboStateChanged(const FComboState& NewComboState)
{
	if (NewComboState != FComboState::None())
	{
		// 50% chance of not doing a follow up
		if (FMath::RandHelper(2) == 0)
		{
			return;
		}
		if (CharacterAsc.IsValid())
		{
			FGameplayTag FollowUpAbilityTag;
			int32 InputId = static_cast<int32>(EMeleeInputID::Attack);
			if (NewComboState.GetGameplayTagFromInput(InputId, FollowUpAbilityTag))
			{
				CharacterAsc->QueueAbility(FollowUpAbilityTag);
				GetWorld()->GetTimerManager().SetTimer(InternalAttackCooldown,
					FTimerDelegate::CreateUObject(this, &AEnemyController::EndInternalCooldown),
					SuccessfulAttackCooldown, false);
				return;
			}
			InputId = static_cast<int32>(EMeleeInputID::Secondary);
			if (NewComboState.GetGameplayTagFromInput(InputId, FollowUpAbilityTag))
			{
				CharacterAsc->QueueAbility(FollowUpAbilityTag);
				GetWorld()->GetTimerManager().SetTimer(InternalAttackCooldown,
					FTimerDelegate::CreateUObject(this, &AEnemyController::EndInternalCooldown),
					SuccessfulAttackCooldown, false);
				return;
			}
		}
	}
}

TArray<TWeakObjectPtr<AActor>> AEnemyController::GetTargets() const
{
	TArray<TWeakObjectPtr<AActor>>Targets;
	if (UWorld* World = GetWorld())
	{
		if (ULevel* Level = World->GetCurrentLevel())
		{
			for (AActor* Actor : Level->Actors)
			{
				if (IsTarget(Actor))
				{
					Targets.Add(Actor);
				}
			}
		}
	}
	return Targets;
}


bool AEnemyController::IsTarget(AActor* Target) const
{
	if (!Target)
	{
		return false;
	}
	if (Target->IsA(ACharacter::StaticClass()))
	{
		// Just some simple control -- do the checks in order of complexity, from least to greatest
		if (Target == GetPawn()) { return false; }
		return IsValidTarget(Target);
	}
	return false;
}

bool AEnemyController::IsValidTarget_Implementation(AActor* Target) const
{
	return true;
}

TWeakObjectPtr<AActor> AEnemyController::GetClosestTarget() const
{
	if (Character.IsValid())
	{
		const FVector CurrentLocation = Character->GetActorLocation();
		TArray<TWeakObjectPtr<AActor>> Targets = PotentialTargets;
		if (Targets.IsEmpty())
		{
			return nullptr;
		}
		bool bFoundTarget = false;
		TWeakObjectPtr<AActor> ClosestTarget = nullptr;
		float MinDist = 0.0f;

		const float MaxViewDistSquared = MaxViewDistance * MaxViewDistance;
		for (TWeakObjectPtr<AActor> Target : Targets)
		{
			const float CurrentDist = FVector::DistSquared(Target->GetActorLocation(), CurrentLocation);
			if (!bFoundTarget)
			{
				if (CurrentDist < MaxViewDistSquared)
				{
					MinDist = CurrentDist;
					ClosestTarget = Target;
				}
			}
			else if (CurrentDist < MinDist && CurrentDist <= MaxViewDistSquared)
			{
				MinDist = CurrentDist;
				ClosestTarget = Target;
			}
		}
		return ClosestTarget;
	}
	return nullptr;
}
