// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaravanGameplayAbility.h"
#include "Abilities/GameplayAbility.h"

#include "UObject/Object.h"
#include "TestMelee_GA.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UTestMelee_GA : public UCaravanGameplayAbility
{
	GENERATED_BODY()

	UTestMelee_GA();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION()
	void HandleMontageEnd();

	UFUNCTION()
	void HitTarget(const FHitResult& HitResult);

	void ApplyCancelEffect(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetHit(const FHitResult& HitResult);

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	TSubclassOf<UGameplayEffect> DurationEffect;

	// Since effect removal is not predicted, apply an effect to cancel the initial effect
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	TSubclassOf<UGameplayEffect> DurationCancelEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	TSubclassOf<UGameplayEffect> HitEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	UAnimMontage* MeleeAnimation;
};