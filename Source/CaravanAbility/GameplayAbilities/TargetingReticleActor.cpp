// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingReticleActor.h"

#include "CaravanAbility/CaravanAbility.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATargetingReticleActor::ATargetingReticleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
}


void ATargetingReticleActor::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ATargetingReticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (MovementMode)
	{
	case EReticleMovementMode::FollowOnwer:
	{
		if (OwningPawn)
		{
			const FVector NewLocation = OwningPawn->GetActorLocation();
			const FRotator NewRotation = OwningPawn->GetActorRotation();
			SetActorLocationAndRotation(NewLocation, NewRotation);
		}
		else
		{
			UE_LOG(LogTargetingSystem, Warning, TEXT("Unable to find valid target for reticle."));
		}
		break;
	}
	case EReticleMovementMode::FollowInput:
	{
		if (!HasLocalNetOwner())
		{
			SetActorLocation(GetSmoothedLocation(DeltaTime));
			return;
		}
		FVector InputVector = FVector::Zero();
		if (OwningPawn)
		{
			InputVector = OwningPawn->GetLastMovementInputVector();
		}
		else
		{
			UE_LOG(LogTargetingSystem, Warning, TEXT("%s reticle '%s' unable to find owner."),
				GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"),
				*GetName());
		}
		const FVector NewLocation = GetActorLocation() + InputVector * MovementSpeed * DeltaTime;
		SetActorLocation(NewLocation);

		if (HasLocalNetOwner())
		{
			ServerSetLocation(NewLocation);
		}

		break;
	}
	case EReticleMovementMode::Static:
	{
		if (!HasLocalNetOwner())
		{
			SetActorLocation(GetSmoothedLocation(DeltaTime));
		}
		break;
	}
	}
}


void ATargetingReticleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ATargetingReticleActor, OwningPawn);
	DOREPLIFETIME_CONDITION(ATargetingReticleActor, MovementMode, COND_OwnerOnly);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void ATargetingReticleActor::PostNetReceiveLocationAndRotation()
{
	const FRepMovement& LocalRepMovement = GetReplicatedMovement();
	FVector NewLocation = FRepMovement::RebaseOntoLocalOrigin(LocalRepMovement.Location, this);

	if (RootComponent && RootComponent->IsRegistered() && (NewLocation != GetActorLocation() || LocalRepMovement.Rotation != GetActorRotation()))
	{
		DesiredLocation = NewLocation;
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


void ATargetingReticleActor::ServerSetLocation_Implementation(FVector Location)
{
	DesiredLocation = Location;
	GetReplicatedMovement_Mutable().Location = Location;
}


void ATargetingReticleActor::FadeIn_Implementation()
{

	if (OwningPawn)
	{
		SetActorLocationAndRotation(OwningPawn->GetActorLocation(), OwningPawn->GetActorRotation());
	}
	MovementMode = EReticleMovementMode::FollowInput;
	OnFadeIn();
}


void ATargetingReticleActor::FadeOut_Implementation()
{
	MovementMode = EReticleMovementMode::Static;
	if (HasLocalNetOwner())
	{
		ServerSetLocation(GetActorLocation());
	}
	OnFadeOut();
}


void ATargetingReticleActor::SetOwningPawn(APawn* NewPawn)
{
	OwningPawn = NewPawn;
}


void ATargetingReticleActor::OnRep_SetOwningPawn()
{
	if (OwningPawn)
	{
		if (!InputComponent)
		{
			InputComponent = OwningPawn->InputComponent;
		}
		else
		{
			UE_LOG(LogTargetingSystem, Warning, TEXT("%s Attempting to set InputComponent on 'BeginPlay' for reticle '%s', but already has an InputComponent"),
				GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"),
				*GetName());
		}
	}
	else
	{
		UE_LOG(LogTargetingSystem, Warning, TEXT("%s Attempting to set InputComponent for reticle '%s', but pawn was invalid."),
			GetNetMode() == ENetMode::NM_Client ? TEXT("[client]") : TEXT("[server]"),
			*GetName());
	}
}


FVector ATargetingReticleActor::GetSmoothedLocation(const float DeltaTime) const
{
	const float DistSquared = FVector::DistSquared(DesiredLocation, GetActorLocation());
	const float MinDistSquared = FMath::Square(MinDelay * MovementSpeed);
	const float MaxDistSquared = FMath::Square(MaxDelay * MovementSpeed);
	if (DistSquared >= MaxDistSquared)
	{
		return DesiredLocation;
	}
	float AdjustedMovementSpeed = MovementSpeed;
	if (DistSquared >= MinDistSquared)
	{
		const float AdjustmentFactor = FMath::GetMappedRangeValueClamped<float>(TRange<float>(MinDistSquared, MaxDistSquared), TRange<float>(1.0f, 2.0f), DistSquared);
		AdjustedMovementSpeed *= AdjustmentFactor;
	}
	return FMath::VInterpConstantTo(GetActorLocation(), DesiredLocation, DeltaTime, AdjustedMovementSpeed);
}
