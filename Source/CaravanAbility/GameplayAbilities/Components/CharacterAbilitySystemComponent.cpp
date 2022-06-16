// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "CaravanAbility/GameplayAbilities/CaravanGameplayAbility.h"
#include "CaravanAbility/CaravanAbility.h"

#include "Net/UnrealNetwork.h"


UCharacterAbilitySystemComponent::UCharacterAbilitySystemComponent()
{
	/* OnGameplayEffectAppliedDelegateToSelf.AddWeakLambda(this, [this](UAbilitySystemComponent*, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle) {
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
	}); */
}


void UCharacterAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(UCharacterAbilitySystemComponent, ComboStateName, COND_SimulatedOnly);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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


void UCharacterAbilitySystemComponent::QueueAbility(const FGameplayTag& AbilityTag)
{
	UE_LOG(LogAbilityQueue, Display, TEXT("[%s] Ability queued: '%s'"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"), *AbilityTag.ToString());
	QueuedAbilities.Enqueue(TPair<float, FGameplayTag>(GetWorld()->TimeSeconds, AbilityTag));
}


void UCharacterAbilitySystemComponent::SetNextComboAbility(FName NewComboState)
{
	ComboStateName = NewComboState;
	SetComboStateFromName(ComboStateName);
}


void UCharacterAbilitySystemComponent::SetComboStateFromName(FName Name)
{
	for (FComboState& ComboState : ComboStates)
	{
		if (ComboState.Name == Name)
		{
			CurrentComboState = ComboState;
			return;
		}
	}
	UE_LOG(LogAbilityQueue, Warning, TEXT("ComboState set to '%s', but could not find an FComboState with that name."), *Name.ToString());
}


bool UCharacterAbilitySystemComponent::GetNextValidQueuedAbility(FGameplayTag& OutAbilityTag)
{
	if (QueuedAbilities.IsEmpty())
	{
		UE_LOG(LogAbilityQueue, Display, TEXT("[%s] No entries for abilities found in queue"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"));
		return false;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	TPair<float, FGameplayTag> QueuedAbility;
	while (QueuedAbilities.Dequeue(QueuedAbility))
	{
		const float AbilityQueuedTime = QueuedAbility.Key;
		if (CurrentTime - AbilityQueuedTime <= AbilityLifetime)
		{
			OutAbilityTag = QueuedAbility.Value;
			return true;
		}
		else
		{
			UE_LOG(LogAbilityQueue, Display, TEXT("Ability '%s' in queue expired."), *OutAbilityTag.ToString())
		}
	}
	UE_LOG(LogAbilityQueue, Display, TEXT("[%s] No valid abilities found in queue"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy") );
	return false;
}


bool UCharacterAbilitySystemComponent::RemoveActiveGameplayEffect(FActiveGameplayEffectHandle Handle,
	int32 StacksToRemove)
{
	return Super::RemoveActiveGameplayEffect(Handle, StacksToRemove);
}


void UCharacterAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	FGameplayTag NextAbility;
	if (GetNextValidQueuedAbility(NextAbility))
	{
		if (TryActivateAbilitiesByTag(FGameplayTagContainer(NextAbility)))
		{
			UE_LOG(LogAbilityQueue, Display, TEXT("[%s] Ability retrieved from queue: '%s'"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"), *NextAbility.ToString());
		}
		else
		{
			UE_LOG(LogAbilityQueue, Display, TEXT("[%s] Ability '%s' retrieved from queue, but couldn't be activated"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"), *NextAbility.ToString());
		}
	}
}


void UCharacterAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	FGameplayTag AbilityTag;
	if (CurrentComboState.GetGameplayTagFromInput(InputID, AbilityTag) )
	{
		for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.IsActive())
			{
				UE_LOG(LogAbilityQueue, Display, TEXT("[%s] Ability '%s' currently active, queuing ability '%s'"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"), *AbilitySpec.Ability->GetName(), *AbilityTag.ToString());
				QueueAbility(AbilityTag);
				return;
			}
		}
		// If there are no abilities active, don't try to add this to the queue
		// Since the queue is only checked when an ability ends,
		// adding it to the queue with no active ability
		// means it will never get called
		TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));
	}
	else
	{
		Super::AbilityLocalInputPressed(InputID);
	}
}