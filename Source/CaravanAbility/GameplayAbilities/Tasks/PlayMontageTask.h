// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "PlayMontageTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayMontageTaskDelegate);

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UPlayMontageTask : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()

	UPlayMontageTask(const FObjectInitializer& ObjectInitializer);


public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "PlayMontageAndWait",
	HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UPlayMontageTask* CreatePlayMontageProxy(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, UAnimMontage* Montage, FName StartSectionT = NAME_None, bool bStopWhenAbilityEndsT = true, float AnimRootMotionTranslationScaleT = 1.f, float StartTimeSecondsT = 0.f);


	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	void SharedInitSimulatedTask();

	virtual void TickTask(float DeltaTime) override;

	FPlayMontageTaskDelegate OnFinish;

private:
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(Replicated)
	float Duration;

	bool bIsFinished;

	float StartTime;

	float EndTime;
};
