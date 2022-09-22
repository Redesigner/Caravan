// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayInteraction.h"
#include "CaravanAbility/Character/CharacterBase.h"


FGameplayInteraction::FGameplayInteraction()
	:Instigator(nullptr), Payload(TEXT("None")), InteractionType(EGameplayInteractionType::Talk)
{
}

FGameplayInteraction::FGameplayInteraction(ACharacterBase* Instigator, FName Payload, EGameplayInteractionType InteractionType)
	:Instigator(Instigator), Payload(Payload), InteractionType(InteractionType)
{
}

FGameplayInteraction::FGameplayInteraction(ACharacterBase* Instigator, EGameplayInteractionType InteractionType)
	:Instigator(Instigator), Payload(TEXT("None")), InteractionType(InteractionType)
{
}
