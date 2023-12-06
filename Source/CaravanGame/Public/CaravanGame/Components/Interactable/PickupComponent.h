// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h" 
#include "PickupComponent.generated.h"

UCLASS()
class CARAVANGAME_API UPickupComponent : public UInteractableComponent
{
    GENERATED_BODY()

public:
    UPickupComponent();
    ~UPickupComponent();

    virtual void Interact(class ACharacterBase* Instigator) override;
};