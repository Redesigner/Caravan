// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/Components/Interactable/DialogComponent.h"

#include "CaravanGame/Dialog/DialogHandler.h"
#include "CaravanGame/Character/CharacterBase.h"
#include "CaravanGame/Player/CaravanPlayerController.h"

UDialogComponent::UDialogComponent()
{
}


UDialogComponent::~UDialogComponent()
{
}


void UDialogComponent::Interact(ACharacterBase* Instigator)
{
	ACaravanPlayerController* CaravanPlayerController = Cast<ACaravanPlayerController>(Instigator->GetController());
	if (!CaravanPlayerController)
	{
		return;
	}
	UDialogHandler* DialogHandler = CaravanPlayerController->GetComponentByClass<UDialogHandler>();
	if (!DialogHandler)
	{
		return;
	}
	DialogHandler->SetDialogTable(DialogTable);
	DialogHandler->QueueDialog(TEXT("Start"));
}
