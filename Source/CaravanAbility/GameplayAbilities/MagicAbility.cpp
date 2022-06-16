// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicAbility.h"

void UMagicAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
}

void UMagicAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UMagicAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}
