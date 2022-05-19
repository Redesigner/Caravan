// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbilitySystemComponent.h"

UCharacterAbilitySystemComponent::UCharacterAbilitySystemComponent()
{
	OnGameplayEffectAppliedDelegateToSelf.AddWeakLambda(this, [this](UAbilitySystemComponent*, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle) {
		const FDateTime CurrentTime = FDateTime::Now();
		int TimeMS = CurrentTime.GetMillisecond();

		if (IsOwnerActorAuthoritative())
		{
			UE_LOG(LogAbilitySystem, Display, TEXT("Server copy of effect %s applied at %s : %i"), *GameplayEffectSpec.ToSimpleString(), *CurrentTime.ToString(), TimeMS);
		}
		else
		{
			UE_LOG(LogAbilitySystem, Display, TEXT("Client copy of effect %s applied at %s : %i"), *GameplayEffectSpec.ToSimpleString(), *CurrentTime.ToString(), TimeMS);
		}
	});
}


