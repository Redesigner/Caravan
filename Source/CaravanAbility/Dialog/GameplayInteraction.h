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
	Talk			UMETA(DisplayName = "Talk"),
	ShowDialog		UMETA(DisplayName = "Show Dialog"),
	EndDialog		UMETA(DisplayName = "End Dialog"),
	Respond			UMETA(DisplayName = "Respond")
};

class ACharacterBase;

USTRUCT(BlueprintType)
struct FGameplayInteraction
{
	GENERATED_BODY()

	FGameplayInteraction();
	FGameplayInteraction(ACharacterBase* Instigator, FName Payload, EGameplayInteractionType InteractionType);
	FGameplayInteraction(ACharacterBase* Instigator, EGameplayInteractionType InteractionType);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACharacterBase* Instigator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Payload;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EGameplayInteractionType> InteractionType;	
};


