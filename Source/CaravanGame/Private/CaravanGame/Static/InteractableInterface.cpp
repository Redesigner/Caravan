// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/Static/InteractableInterface.h"

// Add default functionality here for any IInteractableInterface functions that are not pure virtual.

FGameplayInteraction IInteractableInterface::HandleInteraction(const FGameplayInteraction& Interaction)
{
	return FGameplayInteraction::None();
}
