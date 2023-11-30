// Copyright (c) 2023 Stephen Melnick


#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"

#include "CaravanAbility/GameplayAbilities/Abilities/CaravanGameplayAbility.h"
#include "CaravanAbility/GameplayAbilities/Components/HitboxController.h"
#include "CaravanAbility.h"

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
	QueuedAbilities.Push(TPair<float, FGameplayTag>(GetWorld()->TimeSeconds, AbilityTag));
}

void UCharacterAbilitySystemComponent::ClearAbilityQueue()
{
	QueuedAbilities.Empty();
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
			OnComboStateChanged.Broadcast(ComboState);
			return;
		}
	}
	// UE_LOG(LogAbilityQueue, Warning, TEXT("ComboState set to '%s', but could not find an FComboState with that name."), *Name.ToString());
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
	while (!QueuedAbilities.IsEmpty())
	{
		QueuedAbility = QueuedAbilities.Pop();
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

TArray<TPair<float, FGameplayTag>> UCharacterAbilitySystemComponent::GetQueuedAbilities() const
{
	return QueuedAbilities;
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
		TArray<FGameplayAbilitySpec*> MatchingAbilities;
		GetActivatableGameplayAbilitySpecsByAllMatchingTags(NextAbility.GetSingleTagContainer(), MatchingAbilities);
		if (MatchingAbilities.IsEmpty())
		{
			UE_LOG(LogAbilityQueue, Display, TEXT("[%s] No ability matching tag: '%s' is currently activatable."), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"), *NextAbility.ToString());
			HitboxController->SetCurrentGameplayAbilitySpec(nullptr);
			return;
		}
		// Right now, we just try to activate the *first* ability matching the tag.
		// We only want to activate one ability of this type at a time, that's why we used a queue!
		if (TryActivateAbility(MatchingAbilities[0]->Handle))
		{
			UE_LOG(LogAbilityQueue, Display, TEXT("[%s] Ability retrieved from queue: '%s'"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"), *NextAbility.ToString());
			if (HitboxController.IsValid())
			{
				// Let our HitboxController (if we have one) know that our current *primary* ability is this new one.
				// This is necessary for the hitboxes to respond back to our attack, etc.
				HitboxController->SetCurrentGameplayAbilitySpec(MatchingAbilities[0]);
				return;
			}
		}
		else
		{
			UE_LOG(LogAbilityQueue, Display, TEXT("[%s] Ability '%s' retrieved from queue, but couldn't be activated"), GetOwnerRole() == ENetRole::ROLE_Authority ? TEXT("Authority") : TEXT("Proxy"), *NextAbility.ToString());
		}
	}
	else if (HitboxController.IsValid())
	{
		HitboxController->SetCurrentGameplayAbilitySpec(nullptr);
	}
}


void UCharacterAbilitySystemComponent::AbilityLocalInputPressed(int32 InputId)
{
	UE_LOG(LogAbilitySystemComponent, Display, TEXT("Input %i pressed."), InputId)
	FGameplayTag AbilityTag;
	if (CurrentComboState.GetGameplayTagFromInput(InputId, AbilityTag) )
	{
		// If we couldn't activate the ability, add it to the queue
		if (!TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer()) )
		{
			QueueAbility(AbilityTag);
			return;
		}
		return;
	}
	UE_LOG(LogAbilitySystemComponent, Display, TEXT("No combo state for input %i, using default bindings."), InputId)
	Super::AbilityLocalInputPressed(InputId);
}

void UCharacterAbilitySystemComponent::SetHitboxController(UHitboxController* Controller)
{
	HitboxController = Controller;
}
