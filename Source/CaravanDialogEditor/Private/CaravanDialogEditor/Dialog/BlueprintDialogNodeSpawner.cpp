// Copyright (c) 2023 Stephen Melnick


#include "CaravanDialogEditor/Dialog/BlueprintDialogNodeSpawner.h"

#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "K2Node_FunctionEntry.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BlueprintNodeTemplateCache.h"


// A lot of this code is copied from/slightly modified from the BlueprintEventNodeSpawner code

#define LOCTEXT_NAMESPACE "BlueprintDialogNodeSpawner"

// ----------------------Static Helper Method-----------------------------------------------------
namespace UBlueprintDialogNodeSpawnerImpl
{
	static void RemoveGhostNodes(UEdGraphNode* InNode, UEdGraph* InParentGraph);
}

static void UBlueprintDialogNodeSpawnerImpl::RemoveGhostNodes(UEdGraphNode* InNode, UEdGraph* InParentGraph)
{
	if (InNode && InNode->IsAutomaticallyPlacedGhostNode())
	{
		// Go through all pin connections and consume any disabled nodes so we do not leave garbage.
		for (UEdGraphPin* Pin : InNode->Pins)
		{
			TArray<UEdGraphPin*> LinkedToCopy = Pin->LinkedTo;
			for (UEdGraphPin* OtherPin : LinkedToCopy)
			{
				// Break the pin link back
				OtherPin->BreakLinkTo(Pin);
				RemoveGhostNodes(OtherPin->GetOwningNode(), InParentGraph);
			}
		}

		InNode->BreakAllNodeLinks();
		InParentGraph->RemoveNode(InNode);
	}
};
// ----------------------------------------------------------------------------------------------



UBlueprintDialogNodeSpawner* UBlueprintDialogNodeSpawner::Create(UFunction const* const EventFunc, UObject* Outer/* = nullptr*/)
{
	check(EventFunc != nullptr);

	if (Outer == nullptr)
	{
		Outer = GetTransientPackage();
	}

	UBlueprintDialogNodeSpawner* NodeSpawner = NewObject<UBlueprintDialogNodeSpawner>(Outer);
	NodeSpawner->EventFunc = EventFunc;
	NodeSpawner->NodeClass = UK2Node_Event::StaticClass();

	FBlueprintActionUiSpec& MenuSignature = NodeSpawner->DefaultMenuSignature;
	FText const FuncName = UEdGraphSchema_K2::GetFriendlySignatureName(EventFunc);
	MenuSignature.MenuName = FText::Format(LOCTEXT("EventWithSignatureName", "Dialog Event {0}"), FuncName);
	MenuSignature.Category = UK2Node_CallFunction::GetDefaultCategoryForFunction(EventFunc, LOCTEXT("AddDialogEventCategory", "Add Dialog Event"));
	MenuSignature.Keywords = UK2Node_CallFunction::GetKeywordsForFunction(EventFunc);
	if (MenuSignature.Keywords.IsEmpty())
	{
		MenuSignature.Keywords = FText::FromString(TEXT(" "));
	}
	MenuSignature.Icon = FSlateIcon("EditorStyle", "GraphEditor.Event_16x");

	return NodeSpawner;
}

UBlueprintDialogNodeSpawner* UBlueprintDialogNodeSpawner::Create(TSubclassOf<UK2Node_Event> NodeClass, FName CustomEventName, UObject* Outer)
{
	if (Outer == nullptr)
	{
		Outer = GetTransientPackage();
	}

	UBlueprintDialogNodeSpawner* NodeSpawner = NewObject<UBlueprintDialogNodeSpawner>(Outer);
	NodeSpawner->NodeClass = NodeClass;
	NodeSpawner->CustomEventName = CustomEventName;

	FBlueprintActionUiSpec& MenuSignature = NodeSpawner->DefaultMenuSignature;
	if (CustomEventName.IsNone())
	{
		MenuSignature.MenuName = LOCTEXT("AddDialogEventCategory", "Add Dialog Event...");
		MenuSignature.Icon = FSlateIcon("EditorStyle", "GraphEditor.CustomEvent_16x");
	}
	else
	{
		FText const EventName = FText::FromName(CustomEventName);
		MenuSignature.MenuName = FText::Format(LOCTEXT("EventWithSignatureName", "Dialog Event {0}"), EventName);
		MenuSignature.Icon = FSlateIcon("EditorStyle", "GraphEditor.Event_16x");
	}
	return NodeSpawner;
}


FBlueprintNodeSignature UBlueprintDialogNodeSpawner::GetSpawnerSignature() const
{
	FBlueprintNodeSignature SpawnerSignature(NodeClass);
	SpawnerSignature.AddSubObject(const_cast<UFunction*>(ToRawPtr(EventFunc)));
	return SpawnerSignature;
}


UEdGraphNode* UBlueprintDialogNodeSpawner::Invoke(UEdGraph* ParentGraph, FBindingSet const& Bindings, FVector2D const Location) const
{
	check(ParentGraph != nullptr);
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(ParentGraph);

	UK2Node_Event* DialogNode = nullptr;
	if (!FBlueprintNodeTemplateCache::IsTemplateOuter(ParentGraph))
	{
		UK2Node_Event const* PreExistingNode = FindPreExistingEvent(Blueprint, Bindings);
		DialogNode = const_cast<UK2Node_Event*>(PreExistingNode);
	}

	// check(EventFunc != nullptr);

	for (UEdGraph* FuncGraph : Blueprint->FunctionGraphs)
	{
		// Instead of comparing the FunctionName, this variant uses the DialogId as the name
		if (FuncGraph && FuncGraph->GetFName() == DialogId)
		{
			TArray<UK2Node_FunctionEntry*> FunctionEntry;
			FuncGraph->GetNodesOfClass<UK2Node_FunctionEntry>(FunctionEntry);
			if (FunctionEntry.Num())
			{
				return FunctionEntry[0];
			}
			else
			{
				return nullptr;
			}
		}
	}

	if (DialogNode && DialogNode->IsAutomaticallyPlacedGhostNode())
	{
		UBlueprintDialogNodeSpawnerImpl::RemoveGhostNodes(DialogNode, ParentGraph);
		DialogNode = nullptr;
	}

	if (DialogNode == nullptr)
	{
		auto PostSpawnLambda = [](UEdGraphNode* NewNode, bool bInIsTemplateNode, UFunction const* InEventFunc, FName InEventName, FCustomizeNodeDelegate UserDelegate)
		{
			UK2Node_Event* K2EventNode = CastChecked<UK2Node_Event>(NewNode);
			if (InEventFunc != nullptr)
			{
				K2EventNode->EventReference.SetFromField<UFunction>(InEventFunc, false);
				K2EventNode->bOverrideFunction = true;
			}
			else if (!bInIsTemplateNode)
			{
				K2EventNode->CustomFunctionName = InEventName;
			}
			UserDelegate.ExecuteIfBound(NewNode, bInIsTemplateNode);
		};
		FCustomizeNodeDelegate PostSpawnDelegate = FCustomizeNodeDelegate::CreateStatic(PostSpawnLambda, ToRawPtr(EventFunc), DialogId, CustomizeNodeDelegate);
		DialogNode = Super::SpawnNode<UK2Node_Event>(NodeClass, ParentGraph, Bindings, Location, PostSpawnDelegate);
	}
	return DialogNode;
}


UK2Node_Event const* UBlueprintDialogNodeSpawner::FindPreExistingEvent(UBlueprint* Blueprint, FBindingSet const& /*Bindings*/) const
{
	UK2Node_Event* PreExistingNode = nullptr;

	check(Blueprint != nullptr);
	if (!EventFunc)
	{
		return nullptr;
	}
	UClass* ClassOwner = EventFunc->GetOwnerClass()->GetAuthoritativeClass();

	PreExistingNode = FBlueprintEditorUtils::FindOverrideForFunction(Blueprint, ClassOwner, EventFunc->GetFName());

	return PreExistingNode;
}