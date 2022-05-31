// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxController.h"

void UHitboxController::BeginPlay()
{
	Super::BeginPlay();
}

// Add default functionality here for any IHitboxController functions that are not pure virtual.
TWeakObjectPtr<USphereComponent> UHitboxController::GetHitboxByName(FName Name)
{
	for (USceneComponent* SceneComponent : GetAttachChildren())
	{
		if (USphereComponent* SphereComponent = Cast<USphereComponent>(SceneComponent))
		{
			if (SphereComponent->GetFName() == Name)
			{
				return SphereComponent;
			}
		}
	}
	return nullptr;
}

TWeakObjectPtr<USphereComponent> UHitboxController::SpawnHitbox(FName Name, FVector HitboxRelativeLocation, float Radius)
{
	USphereComponent* Hitbox = NewObject<USphereComponent>(this, Name);
	Hitbox->SetSphereRadius(Radius, false);
	Hitbox->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	Hitbox->SetRelativeLocation(HitboxRelativeLocation, false, nullptr, ETeleportType::ResetPhysics);
	Hitbox->SetVisibility(true);
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
	TWeakObjectPtr<USphereComponent> Hitbox = GetHitboxByName(Name);
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
	if (UShapeComponent* ShapeComponent = Cast<UShapeComponent>(OverlappedComponent))
	{
		// ShapeComponent->SetVisibility(false);
	}
	for (AActor* Target : QueuedHitTargets)
	{
		// Only accumulate the hit if the target actor has not already been hit in this ability.
		// Disable this check for multi-hit attacks
		if (OtherActor == Target)
		{
			if (GetOwnerRole() == ENetRole::ROLE_Authority)
			{
				// UE_LOG(LogTemp, Display, TEXT("%s hit again, ignoring extra hit."), *Target->GetName());
			}
			return;
		}
	}
	FHitResult ModifiedHit = FHitResult(SweepResult);
	ModifiedHit.Location = OverlappedComponent->GetComponentLocation();
	HitDetectedDelegate.ExecuteIfBound(ModifiedHit);
	
	QueuedHits.Emplace(ModifiedHit);
	QueuedHitTargets.Emplace(OtherActor);
}

void UHitboxController::ClearOverlaps()
{
	// UE_LOG(LogTemp, Display, TEXT("[%s] Clearing Hitbox Controller of %s... %i hits removed, %i hit targets removed"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"),
	// 	*GetOwner()->GetName(), QueuedHits.Num(), QueuedHitTargets.Num() );

	HitDetectedDelegate.Unbind();
	
	QueuedHits.Empty();
	QueuedHitTargets.Empty();
}
