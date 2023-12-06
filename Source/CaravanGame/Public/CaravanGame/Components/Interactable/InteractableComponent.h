// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" 
#include "InteractableComponent.generated.h"

UCLASS()
class CARAVANGAME_API UInteractableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractableComponent();
    ~UInteractableComponent();

    virtual void Interact(class ACharacterBase* Instigator);
};