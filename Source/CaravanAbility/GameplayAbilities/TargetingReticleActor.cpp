// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingReticleActor.h"

#include "CaravanAbility/CaravanAbility.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATargetingReticleActor::ATargetingReticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ATargetingReticleActor::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ATargetingReticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{
		if (APawn* OwnerPawn = Controller->GetPawn())
		{
			switch (MovementMode)
			{
			case EReticleMovementMode::FollowOnwer:
			{
				const FVector NewLocation = OwnerPawn->GetActorLocation();
				const FRotator NewRotation = OwnerPawn->GetActorRotation();
				SetActorLocationAndRotation(OwnerPawn->GetActorLocation(), OwnerPawn->GetActorRotation());
				MulticastUpdateLocation(NewLocation);
				break;
			}
			case EReticleMovementMode::FollowInput:
			{
				const FVector InputVector = OwnerPawn->GetLastMovementInputVector();
				const FVector NewLocation = GetActorLocation() + InputVector * MovementSpeed * DeltaTime;
				SetActorLocation(NewLocation);
				MulticastUpdateLocation(NewLocation);
				break;
			}
			case EReticleMovementMode::Static:
			{
				break;
			}
			}
		}
	}
	else
	{
		// UE_LOG(LogTargetingSystem, Warning, TEXT("%s Unable to move targeting reticle, no controller found."),
		// 	GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"));
	}
}


FVector ATargetingReticleActor::GetGroundLocation()
{
	const float MaxGroundDistance = 250.0f;
	const FVector Origin = GetActorLocation();
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


void ATargetingReticleActor::MulticastUpdateLocation_Implementation(FVector Location)
{
	SetActorLocation(Location);
}


void ATargetingReticleActor::ClientSetLocation_Implementation(FVector Location)
{
	SetActorLocation(Location);
}


void ATargetingReticleActor::MulticastSetReticleMovementMode_Implementation(EReticleMovementMode NewMovementMode)
{
	MovementMode = NewMovementMode;
}


void ATargetingReticleActor::SetController(AController* NewController)
{
	Controller = NewController;
}


void ATargetingReticleActor::FadeIn()
{
	if (Controller)
	{
		if (APawn* OwnerPawn = Controller->GetPawn())
		{
			SetActorLocationAndRotation(OwnerPawn->GetActorLocation(), OwnerPawn->GetActorRotation());
		}
	}
	MulticastSetReticleMovementMode(EReticleMovementMode::FollowInput);
	OnFadeIn();
}


void ATargetingReticleActor::FadeOut()
{
	MulticastSetReticleMovementMode(EReticleMovementMode::Static);
	OnFadeOut();
}
