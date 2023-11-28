// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "CaravanGameplayAbility.h"
#include "CaravanAbility/GameplayAbilities/Combo/ComboState.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

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


	/// <summary>
	/// UFUNCTION callback for ending the ability. This will be called when the Animation Montage associated with this ability ends.
	/// </summary>
	UFUNCTION()
	void EndAbilityManual();

	UFUNCTION()
	void AbilityInterrupted();

	/// <summary>
	/// UFUNCTION callback for applying damage, etc. on the authority
	/// This is passed to the owning character's hitbox controller
	/// </summary>
	UFUNCTION()
	void HitTarget(const FHitResult& HitResult);

	/// <summary>
	/// UFUNCTION callback for handling local effects.
	/// This is where local gameplay cues should be applied.
	/// </summary>
	UFUNCTION()
	void HitTargetLocal(const FHitResult& HitResult);

	/// <summary>
	/// Applies a gameplay cue with params, making sure that the locally predicted cues aren't played twice
	/// </summary>
	/// <param name="HitResult">Optional HitResult params to be used, i.e., location of the hit</param>
	void ApplyGameplayCue(const FHitResult& HitResult);
	
	UFUNCTION()
	void HitInterrupted_Internal(const FHitResult& HitResult);


public:
	UFUNCTION(BlueprintImplementableEvent)
	void HitInterrupted(const FHitResult& HitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetHit(const FHitResult& HitResult);


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	TSubclassOf<UGameplayEffect> HitEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	TSubclassOf<UGameplayEffect> InterruptEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	UAnimMontage* MeleeAnimation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	FName MontageSectionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee)
	FName NextComboState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee, DisplayName = "Tag to apply to self when hit landed")
	FGameplayTag GameplayCueSelf;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee, DisplayName = "Tag to apply to self when hit interrupted")
	FGameplayTag GameplayCueSelfInterrupt;

private:
	class UAbilityTask_PlayMontageAndWait* MontageTask;
};