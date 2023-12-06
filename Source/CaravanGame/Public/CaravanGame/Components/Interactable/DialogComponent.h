// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" 
#include "DialogComponent.generated.h"

UCLASS()
class CARAVANGAME_API UDialogComponent : public UInteractableComponent
{
    GENERATED_BODY()

public:
    UDialogComponent();
    ~UDialogComponent();

    virtual void Interact(class ACharacterBase* Instigator) override;
};