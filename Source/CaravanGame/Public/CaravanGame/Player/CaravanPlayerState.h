// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h" 
#include "CaravanPlayerState.generated.h"

UCLASS()
class CARAVANGAME_API ACaravanPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ACaravanPlayerState();
    ~ACaravanPlayerState();
};