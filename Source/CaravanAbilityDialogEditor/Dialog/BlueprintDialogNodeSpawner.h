// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Templates/SubclassOf.h"
#include "BlueprintNodeSignature.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_Event.h"
#include "BlueprintDialogNodeSpawner.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITYDIALOGEDITOR_API UBlueprintDialogNodeSpawner : public UBlueprintNodeSpawner
{
	GENERATED_BODY()

public:

	static UBlueprintDialogNodeSpawner* Create(UFunction const* const EventFunc, UObject* Outer);
	static UBlueprintDialogNodeSpawner* Create(TSubclassOf<UK2Node_Event> NodeClass, FName CustomEventName, UObject* Outer = nullptr);


	// BlueprintNodeSpawner Interface
	virtual FBlueprintNodeSignature GetSpawnerSignature() const override;
	virtual UEdGraphNode* Invoke(UEdGraph* ParentGraph, FBindingSet const& Bindings, FVector2D const Location) const override;

	UPROPERTY()
	FName DialogId;

private:

	UK2Node_Event const* FindPreExistingEvent(UBlueprint* Blueprint, FBindingSet const&) const;


	UPROPERTY()
	TObjectPtr<UFunction const> EventFunc;

	/** The custom name to configure new event nodes with. */
	UPROPERTY()
	FName CustomEventName;
};
