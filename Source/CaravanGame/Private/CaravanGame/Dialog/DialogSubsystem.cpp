// Copyright (c) 2023 Stephen Melnick



#include "CaravanGame/Dialog/DialogSubsystem.h"

#include "CaravanGame/Dialog/Tags/DialogTagManager.h"
#include "CaravanGame/Dialog/DialogResponseInterface.h"


UDialogSubsystem::UDialogSubsystem(const FObjectInitializer& ObjectInitializer)
{
}

void UDialogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	DialogTagManager = NewObject<UDialogTagManager>(this, TEXT("Dialog Tag Manager"));
	DialogTagManager->LoadFile();
	Super::Initialize(Collection);
}


void UDialogSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UDialogTagManager* UDialogSubsystem::GetTagManager()
{
	return DialogTagManager;
}
