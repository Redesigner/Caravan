// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayInteraction.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EGameplayInteractionType
{
	None			UMETA(DisplayName = "None"),

	// Request dialog if possible
	Talk			UMETA(DisplayName = "Talk"),

	// Display a piece of dialog on the screen
	ShowDialog		UMETA(DisplayName = "Show Dialog"),

	// Close dialog if possible
	EndDialog		UMETA(DisplayName = "End Dialog"),

	// Respond to a piece of dialog with options. Should include a payload of our response
	Respond			UMETA(DisplayName = "Respond"),

	// Give an item to the entity that requested this interaction.
	Give			UMETA(DisplayName = "Give")
};

class IInteractableInterface;

USTRUCT(BlueprintType)
struct FGameplayInteraction
{
	GENERATED_BODY()

	FGameplayInteraction();
	FGameplayInteraction(TScriptInterface<IInteractableInterface> Instigator, FName Payload, EGameplayInteractionType InteractionType);
	FGameplayInteraction(TScriptInterface<IInteractableInterface>Instigator, EGameplayInteractionType InteractionType);

	static FGameplayInteraction None();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TScriptInterface<IInteractableInterface> Instigator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Payload;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EGameplayInteractionType> InteractionType;	
};


