// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayInteraction.h"

#include "CaravanAbility/Character/CharacterBase.h"
#include "CaravanAbility/Static/InteractableInterface.h"


FGameplayInteraction::FGameplayInteraction()
	:Instigator(nullptr), Payload(TEXT("None")), InteractionType(EGameplayInteractionType::Talk)
{
}

FGameplayInteraction::FGameplayInteraction(TScriptInterface<IInteractableInterface> Instigator, FName Payload, EGameplayInteractionType InteractionType)
	:Instigator(Instigator), Payload(Payload), InteractionType(InteractionType)
{
}

FGameplayInteraction::FGameplayInteraction(TScriptInterface<IInteractableInterface> Instigator, EGameplayInteractionType InteractionType)
	:Instigator(Instigator), Payload(TEXT("None")), InteractionType(InteractionType)
{
}

FGameplayInteraction FGameplayInteraction::None()
{
	return FGameplayInteraction(nullptr, EGameplayInteractionType::None);
}
