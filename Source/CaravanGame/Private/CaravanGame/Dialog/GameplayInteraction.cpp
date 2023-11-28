// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/Dialog/GameplayInteraction.h"

#include "CaravanGame/Character/CharacterBase.h"
#include "CaravanGame/Static/InteractableInterface.h"


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
