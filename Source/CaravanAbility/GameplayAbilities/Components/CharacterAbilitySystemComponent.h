// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CaravanAbility/GameplayAbilities/Combo/ComboState.h"
#include "CharacterAbilitySystemComponent.generated.h"

/**
 * 
 */
class UHitboxController;

UCLASS()
class CARAVANABILITY_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UCharacterAbilitySystemComponent();

	// VIRTUAL FUNCTIONS
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// Override to output GameplayEffect information to the log
	virtual bool RemoveActiveGameplayEffect(FActiveGameplayEffectHandle Handle, int32 StacksToRemove) override;

	/// Checks the Ability Queue and attempts to play any abilities *before* calling the default NotifyAbilityEnded logic
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;

	/// Handle the local input for the ability. Will first check if there is a corresponding ability in the current combo state
	/// If the ASC has an ability currently active, will add the Ability Tag to the queue
	/// If there are no matching branches available for the current combo state, handle the input normally
	virtual void AbilityLocalInputPressed(int32 InputID) override;

	void SetHitboxController(UHitboxController* Controller);
	

	// GameplayCue functions
	UFUNCTION(BlueprintCallable, Category = GameplayCue)
	void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

	UFUNCTION(BlueprintCallable, Category = GameplayCue)
	void AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

	UFUNCTION(BlueprintCallable, Category = GameplayCue)
	void RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

	// Combo and Queue functions
	void QueueAbility(const FGameplayTag& AbilityTag);

	void ClearAbilityQueue();

	/// Attempt to set the ComboState by name. Currently checks the local ComboStates array for the FComboState with the corresponding name
	/// <param name="NewComboState">The name of the next combo state</param>
	void SetNextComboAbility(FName NewComboState);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnComboStateChanged, const FComboState&)
	FOnComboStateChanged OnComboStateChanged;


// PROPERTIES
	bool bAbilitiesInitialized = false;

	/// How long Ability tags will live in the queue
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Queue)
	float AbilityLifetime = 0.5f;

private:
	UFUNCTION()
	void SetComboStateFromName(FName Name);

	/// <summary>
	/// Check for the next GameplayTag associated with an ability.
	/// Returns false if there are no tags in the queue, or if the tags have all expired
	/// </summary>
	/// <param name="OutAbilityTag">Reference to tag to be set if found</param>
	bool GetNextValidQueuedAbility(FGameplayTag& OutAbilityTag);

	/// The last time we checked the queue. This value should not be changed
	float QueueLastUpdateTime;

	/// Actual queue for the GameplayAbility Tags.
	/// Pairs of:
	/// Time the tag was added to the queue
	/// Tag itself
	TQueue<TPair<float, FGameplayTag>> QueuedAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Combo, meta = (AllowPrivateAccess = true))
	TArray<FComboState> ComboStates;

	/// The actual current combo state, called by queue and other functions.
	/// Should be set locally, but will also be set when ComboStateName is replicated
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = Combo, Transient, meta = (AllowPrivateAccess = true))
	FComboState CurrentComboState;

	/// A name referring to the combo state, since they can't be replicated over the network
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Replicated, Category = Combo, ReplicatedUsing = SetComboStateFromName, Transient, meta = (AllowPrivateAccess = true))
	FName ComboStateName;

	TWeakObjectPtr<UHitboxController> HitboxController;

};
