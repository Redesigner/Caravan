// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaravanGameplayAbility.h"
#include "UObject/Object.h"
#include "ComboAbility.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UComboAbility : public UCaravanGameplayAbility
{
	GENERATED_BODY()

	UComboAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Combo)
	TSubclassOf<UCaravanGameplayAbility> InitialAbility;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int ComboState = 0;
};
