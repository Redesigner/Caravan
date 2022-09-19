// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "../Character/CharacterBase.h"
#include "GameFramework/Character.h"

AEnemyController::AEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyController::Tick(float DeltaTime)
{
	CurrentTarget = GetClosestTarget();
	MoveToActor(CurrentTarget.Get(), 100.0f);
}


void AEnemyController::BeginPlay()
{
	// Populate our target lists once.
	PotentialTargets = GetTargets();
	GetWorld()->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &AEnemyController::TargetSpawned) );
	Super::BeginPlay();
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
		return Target != GetPawn();
	}
	return false;
}


TWeakObjectPtr<AActor> AEnemyController::GetClosestTarget() const
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector CurrentLocation = ControlledPawn->GetActorLocation();
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
