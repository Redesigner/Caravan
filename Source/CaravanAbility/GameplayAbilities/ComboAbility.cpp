// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboAbility.h"

#include "AbilitySystemComponent.h"

UComboAbility::UComboAbility()
{
}

void UComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo->AbilitySystemComponent.IsValid())
	{
		return;
	}
	const UAbilitySystemComponent* AbSC = ActorInfo->AbilitySystemComponent.Get();
	if (ComboState == 0)
	{
		// AbSC->GiveAbilityAnyActivateOnce()
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UComboAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{ 
}
