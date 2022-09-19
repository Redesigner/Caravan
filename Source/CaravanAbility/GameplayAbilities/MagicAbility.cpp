// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicAbility.h"

#include "CaravanAbility/CaravanAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h" 
#include "AbilitySystemComponent.h"
#include "CaravanAbility/Character/CharacterBase.h"
#include "CaravanAbility/Character/CharacterBaseMovementComponent.h"
#include "Components/CharacterAbilitySystemComponent.h"

UMagicAbility::UMagicAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMagicAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	/* UAbilityTask_WaitInputRelease* WaitReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	FScriptDelegate EndAbilityDelegate;
	EndAbilityDelegate.BindUFunction(this, TEXT("EndAbilityManual") );
	WaitReleaseTask->OnRelease.Add(EndAbilityDelegate);
	WaitReleaseTask->Activate(); */
	if (UWorld* World = GetWorld())
	{
		ActivationTime = World->GetTimeSeconds();
	}

	if (DurationEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DurationEffect);
		AppliedDurationEffect = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}

	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwningActorFromActorInfo()) )
	{
		Character->ShowTargetingReticle();
		Character->PauseMovement();
	}
}


void UMagicAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}


void UMagicAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	if (UWorld* World = GetWorld())
	{
		const float EndTime = World->GetTimeSeconds();
		EndAbilityManual(EndTime - ActivationTime);
	}
}

void UMagicAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogAbilitySystem, Display, TEXT("Held magic ability released/ended"));
	if (AppliedDurationEffect.IsValid())
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		ASC->RemoveActiveGameplayEffect(AppliedDurationEffect);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UMagicAbility::EndAbilityManual(float HeldTime)
{
	FVector SpellLocation = FVector::Zero();

	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwningActorFromActorInfo()))
	{
		SpellLocation = Character->HideTargetingReticle();
		UE_LOG(LogTargetingSystem, Display, TEXT("Reticle removed at '%s'"), *SpellLocation.ToString());
		Character->UnpauseMovement();
	}
	AActor* Owner = GetOwningActorFromActorInfo();

	float AdjustedCastTime = MinCastTime;
	if (Owner->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		// AdjustedCastTime -= 0.05f;
	}
	if (HeldTime >= AdjustedCastTime)
	{
		if (SpellAbility.IsValid())
		{
			UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
			TArray<FGameplayAbilitySpec*> AbilitySpecs;
			ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(SpellAbility.GetSingleTagContainer(), AbilitySpecs);
			if (AbilitySpecs.Num() > 0)
			{
				// We need a new context to hold our location data
				// EventData normally holds a handle to a nullptr
				FGameplayEventData EventData;
				FGameplayEffectContext* AbilityContext = new FGameplayEffectContext();
				EventData.Instigator = Owner;
				EventData.EventTag = SpellAbility;
				EventData.ContextHandle = AbilityContext;
				AbilityContext->AddOrigin(SpellLocation);

				FGameplayAbilityActorInfo ActorInfo = GetActorInfo();
				ASC->TriggerAbilityFromGameplayEvent(AbilitySpecs[0]->Handle, &ActorInfo, SpellAbility, &EventData, *ASC);

				// We allocated the EventData, delete it here
				// Context should be cleaned up automatically -- when casting from pointer to handle
				// a TSharedPtr is created, which will automatically destroy it
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
