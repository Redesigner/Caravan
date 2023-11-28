// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "DialogResponseInterface.generated.h"

/**
 * 
 */


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDialogResponseInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IDialogResponseInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = Dialog, meta = (BlueprintInternalUseOnly = "true"))
	void BlueprintDialogHandler(const FString& Response, ACharacter* Source, bool bIsBeginning);

	UFUNCTION()
	virtual const TArray<FString>& GetResponses() const = 0;
	virtual struct FDialogTagHandle* GetRootTag() = 0;
};
