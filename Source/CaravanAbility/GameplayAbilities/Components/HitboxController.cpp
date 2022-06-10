// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxController.h"

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

void UHitboxController::AccumulateOverlaps(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}
	for (FHitResult HitResult : QueuedHits)
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
	}
	HitDetectedDelegate.ExecuteIfBound(ModifiedHit);
	QueuedHits.Emplace(ModifiedHit);
}

void UHitboxController::ClearOverlaps()
{
	QueuedHits.Empty();
}
