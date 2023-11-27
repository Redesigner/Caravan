// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaravanGameplayAbility.h"
#include "MagicAbility.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UMagicAbility : public UCaravanGameplayAbility
{
	GENERATED_BODY()

public:
	UMagicAbility();

	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void EndAbilityManual(float TimeHeld);


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell)
	FGameplayTag SpellAbility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Effects)
	TSubclassOf<UGameplayEffect> DurationEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell)
	float MinCastTime = 1.0f;

private:
	FActiveGameplayEffectHandle AppliedDurationEffect;

	float ActivationTime = 0.0f;
};
