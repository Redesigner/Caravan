// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "K2Node_Event.h"
#include "K2Node_DialogEvent.generated.h"

/**
 * 
 */
class FBlueprintActionDatabaseRegistrar;
class UEdGraph;

UCLASS()
class CARAVANDIALOGEDITOR_API UK2Node_DialogEvent : public UK2Node_Event
{
	GENERATED_BODY()

	UK2Node_DialogEvent(const FObjectInitializer& ObjectInitializer);
	
	//~ Begin UObject Interface
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	//~ End UEdGraphNode Interface

	//~ Begin UK2Node Interface
	// This is called by the editor on the node's CDO, so it cannot have context-sensitive information normally
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//~ End UK2Node Interface

};
