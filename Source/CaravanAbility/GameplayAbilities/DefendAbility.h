// Copyright 2022 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

#include "Containers/Map.h" 
#include "CaravanAbility/GameplayAbilities/CaravanGameplayAbility.h"
#include "CaravanAbility/GameplayAbilities/Components/ArmorComponent.h"

#include "DefendAbility.generated.h"

USTRUCT(BlueprintType)
struct CARAVANABILITY_API FArmorInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EArmorShape> Shape;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget))
	FVector Position;

	UPROPERTY(EditAnywhere)
	FVector Rotation;

	UPROPERTY(EditAnywhere)
	FVector Dimensions;

	UPROPERTY(EditAnywhere)
	FVector Normal;
};

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UDefendAbility : public UCaravanGameplayAbility
{
	GENERATED_BODY()
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

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
