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

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual TArray<AActor> GetTargets() const;
	
};
