// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "HitboxComponent.generated.h"

/**
 * 
 */
struct FGameplayAbilitySpec;

UCLASS()
class CARAVANABILITY_API UHitboxComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector Direction = FVector::Zero();
	
	const FGameplayAbilitySpec* OwningAbility;
};
