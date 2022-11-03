// Copyright 2022 Stephen Melnick


#include "InteractableInterface.h"

// Add default functionality here for any IInteractableInterface functions that are not pure virtual.

FGameplayInteraction IInteractableInterface::HandleInteraction(const FGameplayInteraction& Interaction)
{
	return FGameplayInteraction::None();
}
