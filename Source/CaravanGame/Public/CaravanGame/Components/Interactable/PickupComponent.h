// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" 
#include "PickupComponent.generated.h"

UCLASS()
class CARAVANGAME_API UPickupComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPickupComponent();
    ~UPickupComponent();

    virtual void Interact(class ACharacterBase* Instigator) = 0;
};