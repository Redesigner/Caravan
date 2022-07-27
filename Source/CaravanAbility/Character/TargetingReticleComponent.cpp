// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingReticleComponent.h"

#include "GameFramework/Character.h"

UTargetingReticleComponent::UTargetingReticleComponent()
{
	SetVisibility(false);
	PrimaryComponentTick.bCanEverTick = true;
}


void UTargetingReticleComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UTargetingReticleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		FVector Offset = Character->GetLastMovementInputVector() * 100.0f;
		AddWorldOffset(Offset * DeltaTime);
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UTargetingReticleComponent::GetGroundLocation() const
{
	const float MaxGroundDistance = 250.0f;
	const FVector Origin = GetComponentLocation();
	if (UWorld* World = GetWorld())
	{
		FHitResult GroundTest;
		FCollisionObjectQueryParams FCOParams;
		FCOParams.AddObjectTypesToQuery(ECC_WorldStatic);
		FCOParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		World->LineTraceSingleByObjectType(GroundTest, Origin, Origin + FVector::UpVector * -MaxGroundDistance, FCOParams);

		if (GroundTest.bBlockingHit)
		{
			return GroundTest.Location;
		}
	}
	return Origin;
}

