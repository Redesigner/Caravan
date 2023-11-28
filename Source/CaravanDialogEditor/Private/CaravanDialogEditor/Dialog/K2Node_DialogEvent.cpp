// Copyright (c) 2023 Stephen Melnick


#include "CaravanDialogEditor/Dialog/K2Node_DialogEvent.h"

#include "EdGraph/EdGraph.h"
#include "Engine/Blueprint.h" 
#include "BlueprintNodeSpawner.h"
#include "BlueprintEventNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "Kismet/GameplayStatics.h" 

#include "CaravanGame/Character/CharacterBase.h"
#include "CaravanGame/Dialog/DialogSubsystem.h"
#include "CaravanGame/Dialog/DialogResponseInterface.h"
#include "CaravanGame/Dialog/Tags/DialogTagManager.h"

#include "CaravanDialogEditor/Dialog/BlueprintDialogNodeSpawner.h"

// Guard against reusing variable names...
#define LOCTEXT_NAMESPACE "K2Node_DialogEvent"

// This is the FName we will use to find our UFunction to bind to our node
static FName DIALOG_BlueprintCustomHandler = TEXT("BlueprintDialogHandler");


UK2Node_DialogEvent::UK2Node_DialogEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Bind the UFunction of our class to this
	// Function is accessed from the CDO, so we can't pass in any unique properties here
	EventReference.SetExternalMember(DIALOG_BlueprintCustomHandler, UDialogResponseInterface::StaticClass());
}


void UK2Node_DialogEvent::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	// These checks are just copied from the GameplayCue Event node
	if (Ar.IsLoading())
	{
		if (Ar.UEVer() < VER_UE4_K2NODE_EVENT_MEMBER_REFERENCE && EventSignatureName_DEPRECATED.IsNone() && EventSignatureClass_DEPRECATED == nullptr)
		{
			EventReference.SetExternalMember(DIALOG_BlueprintCustomHandler, UDialogResponseInterface::StaticClass());
		}
	}
}


FText UK2Node_DialogEvent::GetTooltipText() const
{
	return FText::Format(LOCTEXT("DialogEvent_Tooltip", "Handle Dialog Event {0}"), FText::FromName(CustomFunctionName));
}


FText UK2Node_DialogEvent::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	// CustomFunctionName is set by our PostSpawnDelegate
	return FText::FromName(CustomFunctionName);
}


// This function is called for every single node we generated with UBlueprintNodeSpawner,
// but the function itself is run on the CDO only
// Unless we have another way of checking, we can either have:
//		1) all nodes of this type -- (DialogEventNode)
//		2) no nodes of this type
//
// Called whenever the user right clicks inside the module and brings up the context menu
// If always true, this node will always be displayed for every single blueprint
bool UK2Node_DialogEvent::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	bool bIsCompatible = false;
	if (UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph))
	{
		check(Blueprint->GeneratedClass != nullptr);
		if (Blueprint->GeneratedClass->ImplementsInterface(UDialogResponseInterface::StaticClass()))
		{
			IDialogResponseInterface* DIR = Cast<IDialogResponseInterface>(Blueprint->GeneratedClass.GetDefaultObject());
			const TArray<FString>& CDOResponses = DIR->GetResponses();
			if (CDOResponses.Contains(CustomFunctionName.ToString()))
			{
				bIsCompatible = true;
			}
		}
	}
	// return bIsCompatible && Super::IsCompatibleWithGraph(TargetGraph);
	return true;
}


// This method is invoked for every class in our project
void UK2Node_DialogEvent::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	// Skip registration if this class has already finished generating the action list.
	if (!ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		return;
	}

	// This lambda will be called on PostSpawnDelegate, which is run when the editor is open, or the RefreshAll() method is called
	auto CustomNodeLambda = [](UEdGraphNode* NewNode, bool bIsTemplateNode, FString String)
	{
		UK2Node_DialogEvent* EventNode = CastChecked<UK2Node_DialogEvent>(NewNode);
		// Load the function name into the event node
		EventNode->CustomFunctionName = FName(String);
	};

	check(GEngine);
	UDialogSubsystem* DialogSubsystem = GEngine->GetEngineSubsystem<UDialogSubsystem>();

	for (FString DialogOption : DialogSubsystem->GetTagManager()->GetAllFullTagNames() )
	{
		// Attach our lambda to a new, static, FCustomizeNodeDelegate
		UBlueprintNodeSpawner::FCustomizeNodeDelegate PostSpawnDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomNodeLambda, DialogOption);

		// Create a new event node spawner with this string name, and of our specified class
		UBlueprintDialogNodeSpawner* NodeSpawner = UBlueprintDialogNodeSpawner::Create(GetClass(), FName(DialogOption));
		check(NodeSpawner != nullptr);
		// Attach the FCustomizeNodeDelegate to the new NodeSpawner we just made
		NodeSpawner->CustomizeNodeDelegate = PostSpawnDelegate;
		NodeSpawner->DialogId = FName(DialogOption);

		// Bind the node spawner to the global action registrar
		// the registrar will associate the NodeSpawner we just created with this object's class
		// e.g., UK2Node_DialogEvent
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}

}

#undef LOCTEXT_NAMESPACE
