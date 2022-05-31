// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayMontageTask.h"

#include "Net/UnrealNetwork.h"
#include "AbilitySystemGlobals.h"


UPlayMontageTask::UPlayMontageTask(const FObjectInitializer& ObjectInitializer) :
	UAbilityTask_PlayMontageAndWait(ObjectInitializer)
{
	bTickingTask = true;
	bSimulatedTask = true;
}

void UPlayMontageTask::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UPlayMontageTask, Duration);
}

UPlayMontageTask* UPlayMontageTask::CreatePlayMontageProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* Montage, FName StartSectionT, bool bStopWhenAbilityEndsT, float AnimRootMotionTranslationScaleT, float StartTimeSecondsT)
{
	UPlayMontageTask* MyObj = NewAbilityTask<UPlayMontageTask>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = Montage;
	MyObj->Rate = 1.0f;
	MyObj->StartSection = StartSectionT;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScaleT;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEndsT;
	MyObj->StartTimeSeconds = StartTimeSecondsT;

	MyObj->Duration = Montage->GetPlayLength();
	MyObj->bIsFinished = false;

	MyObj->SharedInitSimulatedTask();

	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(MyObj->Duration);

	return MyObj;
}

void UPlayMontageTask::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
	SharedInitSimulatedTask();
}

void UPlayMontageTask::SharedInitSimulatedTask()
{
	if (AbilitySystemComponent->AbilityActorInfo->MovementComponent.IsValid())
	{
		MovementComponent = Cast<UCharacterMovementComponent>(AbilitySystemComponent->AbilityActorInfo->MovementComponent.Get());
		StartTime = GetWorld()->GetTimeSeconds();
		EndTime = StartTime + Duration;
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("UAbilityTask_ApplyRootMotionConstantForce called in Ability %s with null MovementComponent; Task Instance Name %s."),
			Ability ? *Ability->GetName() : TEXT("NULL"),
			*InstanceName.ToString());
	}
}

void UPlayMontageTask::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);

	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		const bool bIsInfiniteDuration = Duration < 0.f;

		if (!bIsInfiniteDuration)
		{
			// Task has finished
			bIsFinished = true;
			if (!bIsSimulating)
			{
				MyActor->ForceNetUpdate();
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnFinish.Broadcast();
				}
				EndTask();
			}
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}
