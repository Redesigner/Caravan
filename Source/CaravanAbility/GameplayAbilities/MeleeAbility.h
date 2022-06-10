// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CaravanGameplayAbility.h"
#include "Abilities/GameplayAbility.h"

#include "UObject/Object.h"
#include "MeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UMeleeAbility : public UCaravanGameplayAbility
{
	GENERATED_BODY()

	UMeleeAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	void ExecuteAttack( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool InitialAttack = true);


	UFUNCTION()
	void EndAbilityManual();

	UFUNCTION()
	void HitTarget(const FHitResult& HitResult);

	UFUNCTION()
	void HitTargetLocal(const FHitResult& HitResult);
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetHit(const FHitResult& HitResult);

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	TSubclassOf<UGameplayEffect> DurationEffect;

	// Since effect removal is not predicted, apply an effect to cancel the initial effect
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	TSubclassOf<UGameplayEffect> DurationCancelEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	TArray<TSubclassOf<UGameplayEffect>> HitEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	UAnimMontage* MeleeAnimation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Animation)
	FName InitialComboState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Animation)
	FName CurrentComboState;
};