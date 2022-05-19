// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/Object.h"
#include "CharacterBaseMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UCharacterBaseMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual float GetMaxSpeed() const override;

	virtual FRotator GetDeltaRotation(float DeltaTime) const override;

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attributes")
	FGameplayAttribute MovementSpeed;
};
