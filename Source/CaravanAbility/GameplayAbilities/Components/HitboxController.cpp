// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxController.h"

#include "GameplayAbilitySpec.h"

#include "CaravanAbility\GameplayAbilities\Components\ArmorComponent.h"

void UHitboxController::BeginPlay()
{
	Super::BeginPlay();
}

// Add default functionality here for any IHitboxController functions that are not pure virtual.
TWeakObjectPtr<UHitboxComponent> UHitboxController::GetHitboxByName(FName Name)
{
	for (USceneComponent* SceneComponent : GetAttachChildren())
	{
		if (UHitboxComponent* SphereComponent = Cast<UHitboxComponent>(SceneComponent))
		{
			if (SphereComponent->GetFName() == Name)
			{
				return SphereComponent;
			}
		}
	}
	return nullptr;
}

TWeakObjectPtr<UHitboxComponent> UHitboxController::SpawnHitbox(FName Name, FVector HitboxRelativeLocation, FVector Direction, float Radius)
{
	UHitboxComponent* Hitbox = NewObject<UHitboxComponent>(this, Name);
	Hitbox->SetSphereRadius(Radius, false);
	Hitbox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	Hitbox->SetRelativeLocation(HitboxRelativeLocation, false, nullptr, ETeleportType::ResetPhysics);
	Hitbox->Direction = Direction;
	Hitbox->OwningAbility = CurrentAbilitySpec;

	Hitbox->SetVisibility(IsVisible());
	Hitbox->SetHiddenInGame(false);

	Hitbox->SetCollisionProfileName(HitboxCollisionProfile.Name);
	Hitbox->SetGenerateOverlapEvents(true);

	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &UHitboxController::AccumulateOverlaps);
	
	Hitbox->RegisterComponent();

	// Trigger any overlap events that happened when we spawned the hitbox
	Hitbox->UpdateOverlaps();
	return Hitbox;
}

void UHitboxController::RemoveHitboxByName(FName Name)
{
	TWeakObjectPtr<UHitboxComponent> Hitbox = GetHitboxByName(Name);
	if (Hitbox.IsValid())
	{
		Hitbox->DestroyComponent();
	}
}

TWeakObjectPtr<UArmorComponent> UHitboxController::GetArmorByName(FName Name)
{
	for (USceneComponent* SceneComponent : GetAttachChildren())
	{
		if (UArmorComponent* SphereComponent = Cast<UArmorComponent>(SceneComponent))
		{
			if (SphereComponent->GetFName() == Name)
			{
				return SphereComponent;
			}
		}
	}
	return nullptr;
}

TWeakObjectPtr<UArmorComponent> UHitboxController::SpawnArmor(FName Name, FVector ArmorRelativeLocation, float Radius)
{
	UArmorComponent* Armor = NewObject<UArmorComponent>(this, Name);
	Armor->OwningHitboxController = this;
	Armor->SetSphereRadius(Radius, false);
	Armor->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	Armor->SetRelativeLocation(ArmorRelativeLocation, false, nullptr, ETeleportType::ResetPhysics);

	Armor->SetVisibility(IsVisible());
	Armor->SetHiddenInGame(false);

	Armor->SetCollisionProfileName(ArmorCollisionProfile.Name);

	Armor->RegisterComponent();
	return Armor;
}

void UHitboxController::RemoveArmorByName(FName Name)
{
	TWeakObjectPtr<UArmorComponent> Armor = GetArmorByName(Name);
	if (Armor.IsValid())
	{
		Armor->DestroyComponent();
	}
}

void UHitboxController::AccumulateOverlaps(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}
	for (const FHitResult& HitResult : QueuedHits)
	{
		// Only accumulate the hit if the target actor has not already been hit in this ability.
		// Disable this check for multi-hit attacks
		if (OtherActor == HitResult.GetActor())
		{
			return;
		}
	}
	FHitResult ModifiedHit = FHitResult(SweepResult);
	ModifiedHit.Location = OverlappedComponent->GetComponentLocation();
	
	if (UHitboxComponent* HitboxComponent = Cast<UHitboxComponent>(OverlappedComponent))
	{
		ModifiedHit.Normal =  HitboxComponent->GetComponentRotation().RotateVector(HitboxComponent->Direction);

		if (GetIsHitBlocked(HitboxComponent, OtherActor->GetActorLocation() ))
		{
			HitInterruptedDelegate.ExecuteIfBound(ModifiedHit);
			return;
		}
	}
	HitDetectedDelegate.ExecuteIfBound(ModifiedHit);
	QueuedHits.Emplace(ModifiedHit);
}


void UHitboxController::ClearOverlaps()
{
	QueuedHits.Empty();
}

void UHitboxController::SetCurrentGameplayAbilitySpec(const FGameplayAbilitySpec* AbilitySpec)
{
	CurrentAbilitySpec = AbilitySpec;
}


bool UHitboxController::GetIsHitBlocked(const UHitboxComponent* Hitbox, const FVector& HitLocation) const
{
	FHitResult ArmorTestResult;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);
	// const FVector StartLocation = Hitbox->GetComponentLocation();
	const FVector StartLocation = GetOwner()->GetActorLocation();
	const FVector EndLocation = HitLocation;
	GetWorld()->LineTraceSingleByObjectType(ArmorTestResult, StartLocation, EndLocation, QueryParams);
	// We hit some armor!
	// DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 3.0f, FColor::Red, false, 2.0f);
	if (ArmorTestResult.bBlockingHit)
	{
		// This *should* always be true, other components shouldn't be using this object channel
		if (UArmorComponent* ArmorComponent = Cast<UArmorComponent>(ArmorTestResult.GetComponent()))
		{
			ArmorComponent->OwningHitboxController->HitBlockedDelegate.ExecuteIfBound(ArmorTestResult);
			// Hand this check off to the component itself
			return ArmorComponent->CanBlockHit(Hitbox->OwningAbility);
		}
	}
	return false;
}
