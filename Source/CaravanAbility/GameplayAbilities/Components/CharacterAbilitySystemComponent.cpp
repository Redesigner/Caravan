// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"

UCharacterAbilitySystemComponent::UCharacterAbilitySystemComponent()
{
	OnGameplayEffectAppliedDelegateToSelf.AddWeakLambda(this, [this](UAbilitySystemComponent*, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle) {
		const FDateTime CurrentTime = FDateTime::Now();
		int TimeMS = CurrentTime.GetMillisecond();

		if (IsOwnerActorAuthoritative())
		{
			UE_LOG(LogAbilitySystem, Display, TEXT("Server copy of effect %s [%s] applied at %s : %i"), *GameplayEffectSpec.ToSimpleString(), *GameplayEffectHandle.ToString(), *CurrentTime.ToString(), TimeMS);
		}
		else
		{
			UE_LOG(LogAbilitySystem, Display, TEXT("Client copy of effect %s [%s] applied at %s : %i"), *GameplayEffectSpec.ToSimpleString(), *GameplayEffectHandle.ToString(), *CurrentTime.ToString(), TimeMS);
		}
	});
}


void UCharacterAbilitySystemComponent::ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Executed, GameplayCueParameters);
}


void UCharacterAbilitySystemComponent::AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::OnActive, GameplayCueParameters);
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::WhileActive, GameplayCueParameters);
}


void UCharacterAbilitySystemComponent::RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Removed, GameplayCueParameters);
}


bool UCharacterAbilitySystemComponent::RemoveActiveGameplayEffect(FActiveGameplayEffectHandle Handle,
	int32 StacksToRemove)
{
	const FDateTime CurrentTime = FDateTime::Now();
	int TimeMS = CurrentTime.GetMillisecond();

	if (IsOwnerActorAuthoritative())
	{
		UE_LOG(LogAbilitySystem, Display, TEXT("Server attempting to remove effect %s at %s : %i"), *Handle.ToString(), *CurrentTime.ToString(), TimeMS);
	}
	else
	{
		UE_LOG(LogAbilitySystem, Display, TEXT("Client attempting to remove effect %s at %s : %i"), *Handle.ToString(), *CurrentTime.ToString(), TimeMS);
	}
	return Super::RemoveActiveGameplayEffect(Handle, StacksToRemove);
}


