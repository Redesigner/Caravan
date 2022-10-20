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
	bool bIsClose = (MoveToActor(CurrentTarget.Get(), 100.0f) == EPathFollowingRequestResult::AlreadyAtGoal );
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
		FVector DistanceToTarget = CurrentTarget->GetActorLocation() - Character->GetActorLocation();
		DistanceToTarget.Normalize();
		float AngleBetween = FMath::RadiansToDegrees(FMath::Acos(DistanceToTarget.Dot(Character->GetActorForwardVector())) );
		if (AngleBetween <= 35.0f)
		{
			// Attempt to execute an attack
			Character->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ValidAttackTagContainer);
		}
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
		return Target != GetPawn() && IsValidTarget(Target);
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
		TWeakObjectPtr<AActor> ClosestTarget = Targets[0];
		float MinDist = FVector::DistSquared(ClosestTarget->GetActorLocation(), CurrentLocation);
		for (TWeakObjectPtr<AActor> Target : Targets)
		{
			const float CurrentDist = FVector::DistSquared(Target->GetActorLocation(), CurrentLocation);
			if (CurrentDist < MinDist)
			{
				MinDist = CurrentDist;
				ClosestTarget = Target;
			}
		}
		return ClosestTarget;
	}
	return nullptr;
}
