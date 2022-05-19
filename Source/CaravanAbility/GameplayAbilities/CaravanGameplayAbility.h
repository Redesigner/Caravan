// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UObject/Object.h"
#include "CaravanGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UCaravanGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
