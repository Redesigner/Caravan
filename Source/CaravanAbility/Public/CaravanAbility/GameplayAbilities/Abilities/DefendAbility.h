// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

#include "Containers/Map.h" 
#include "CaravanAbility/GameplayAbilities/Abilities/CaravanGameplayAbility.h"
#include "CaravanAbility/GameplayAbilities/Components/ArmorComponent.h"

#include "DefendAbility.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UDefendAbility : public UCaravanGameplayAbility
{
	GENERATED_BODY()
	
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void BlockHit(const FHitResult& HitResult);

	UFUNCTION()
	void BlockHitLocal(const FHitResult& HitResult);


	void ApplyGameplayCue(const FHitResult& HitResult);


public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHitBlocked(const FHitResult& HitResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Effect)
	TSubclassOf<UGameplayEffect> BlockingEffect;

	FActiveGameplayEffectHandle EffectHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Melee, DisplayName = "Tag to apply to self when hit landed")
	FGameplayTag GameplayCueSelf;

private:
	TWeakObjectPtr<class UHitboxController> HitboxController;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray< FArmorInstance > ArmorInstances;
};
