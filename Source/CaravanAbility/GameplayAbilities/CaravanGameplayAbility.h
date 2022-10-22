// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UObject/Object.h"
#include "CaravanGameplayAbility.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMeleeInputID : uint8
{
	Attack		UMETA(DisplayName = "Attack"),
	Secondary	UMETA(DisplayName = "Secondary"),
	Defend		UMETA(DisplayName = "Defend"),
	None		UMETA(DisplayName = "None"),
	Confirm		UMETA(DisplayName = "Confirm"),
	Cancel		UMETA(DisplayName = "Cancel")
};

UCLASS()
class CARAVANABILITY_API UCaravanGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

public:
	UPROPERTY(EditAnywhere, Category = Input)
	EMeleeInputID InputBinding = EMeleeInputID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Input)
	bool bActivateOnInput = true;
};