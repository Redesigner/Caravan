// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API AEnemyController : public AAIController
{
	GENERATED_BODY()

	AEnemyController();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void TargetSpawned(AActor* Target);

protected:
	TArray<TWeakObjectPtr<AActor>> GetTargets() const;
	virtual bool IsTarget(AActor* Target) const;

private:
	TWeakObjectPtr<AActor> GetClosestTarget() const;
	TArray<TWeakObjectPtr<AActor>> PotentialTargets;

	UPROPERTY(VisibleInstanceOnly, Category = Targeting, meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<AActor> CurrentTarget;
};
