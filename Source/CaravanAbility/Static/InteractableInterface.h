// Copyright 2022 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CaravanAbility\Dialog\GameplayInteraction.h"

#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CARAVANABILITY_API IInteractableInterface
{
	GENERATED_BODY()

public:

	// Handle a new interaction, sending a response back to the invoker
	UFUNCTION(BlueprintCallable)
	virtual FGameplayInteraction HandleInteraction(const FGameplayInteraction& Interaction);
};
