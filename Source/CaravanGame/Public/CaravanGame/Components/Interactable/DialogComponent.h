// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h" 
#include "DialogComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CARAVANGAME_API UDialogComponent : public UInteractableComponent
{
    GENERATED_BODY()

public:
    UDialogComponent();
    ~UDialogComponent();

    virtual void Interact(class ACharacterBase* Instigator) override;

private:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Dialog, meta = (AllowPrivateAccess = true))
    UDataTable* DialogTable;
};