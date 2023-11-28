// Copyright (c) 2023 Stephen Melnick

#include "CaravanDialogEditor/CaravanDialogEditorModule.h"

#include "BlueprintGraphModule.h"
#include "BlueprintActionFilter.h"
#include "BlueprintEventNodeSpawner.h"
#include "Modules/ModuleManager.h"

#include "CaravanGame/Dialog/DialogResponseInterface.h"
#include "CaravanGame/Dialog/Tags/DialogTag.h"
#include "CaravanGame/Dialog/Tags/DialogTagHandle.h"
#include "CaravanGame/Dialog/DialogSubsystem.h"

#include "CaravanDialogEditor/Dialog/K2Node_DialogEvent.h"
#include "CaravanDialogEditor/Dialog/BlueprintDialogNodeSpawner.h"
#include "CaravanDialogEditor/Dialog/Slate/DialogTagCustomization.h"

DEFINE_LOG_CATEGORY(LogDialogEditor);

IMPLEMENT_MODULE( FCaravanDialogEditorModule, CaravanDialogEditor );

bool IsActionCompatible(FBlueprintActionFilter const& Filter, FBlueprintActionInfo& BlueprintAction)
{
	if (BlueprintAction.GetNodeClass()->IsChildOf<UK2Node_DialogEvent>())
	{
		if (const UBlueprintDialogNodeSpawner* DialogNodeSpawner = Cast<UBlueprintDialogNodeSpawner>(BlueprintAction.NodeSpawner))
		{
			for (UBlueprint* Blueprint : Filter.Context.Blueprints)
			{
				if (Blueprint->GeneratedClass && Blueprint->GeneratedClass->ImplementsInterface(UDialogResponseInterface::StaticClass()))
				{
					IDialogResponseInterface* DIR = Cast<IDialogResponseInterface>(Blueprint->GeneratedClass.GetDefaultObject());
					const FDialogTagHandle* DialogTag = DIR->GetRootTag();
					check(GEngine);
					UDialogSubsystem* DialogSubsystem = GEngine->GetEngineSubsystem<UDialogSubsystem>();
					check(DialogSubsystem);
					/*
					if (DialogNodeSpawner->DialogId.ToString().StartsWith(DialogTag->GetFullName()) )
					{
						return false;
					}
					*/
					return true;
				}
			}
		}
		return true;
	}
	return false;
}

void FCaravanDialogEditorModule::StartupModule()
{
	FBlueprintGraphModule& BlueprintGraphModule = FModuleManager::LoadModuleChecked<FBlueprintGraphModule>("BlueprintGraph");

	CustomRejectionTest = &BlueprintGraphModule.GetExtendedActionMenuFilters().Add_GetRef(FBlueprintGraphModule::FActionMenuRejectionTest::CreateStatic(IsActionCompatible));

	
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout
	(
		FDialogTagHandle::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDialogTagCustomization::MakeInstance)
	);
	PropertyModule.NotifyCustomizationModuleChanged();
	
}

void FCaravanDialogEditorModule::ShutdownModule()
{
	if (CustomRejectionTest != nullptr)
	{
		FBlueprintGraphModule& BlueprintGraphModule = FModuleManager::LoadModuleChecked<FBlueprintGraphModule>("BlueprintGraph");

		BlueprintGraphModule.GetExtendedActionMenuFilters().RemoveAll([this](const FBlueprintGraphModule::FActionMenuRejectionTest& Delegate)
		{
			return static_cast<const void*>(&Delegate) == CustomRejectionTest;
		});
	}
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout("DialogTag");
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}
