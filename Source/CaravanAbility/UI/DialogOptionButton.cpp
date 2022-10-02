// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogOptionButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CaravanAbility/Player/CaravanPlayerController.h"
#include "CaravanAbility/Dialog/DialogHandler.h"

void UDialogOptionButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (OptionButton)
	{
		OptionButton->OnClicked.AddUniqueDynamic(this, &UDialogOptionButton::SendClick);
	}
}

void UDialogOptionButton::SetText(FString Text)
{
	if (!OptionText)
	{
		return;
	}
	OptionText->SetText(FText::FromString(Text));
	Response = Text;
}

void UDialogOptionButton::SendClick()
{
	if (UDialogHandler* DialogHandler = GetOwningPlayer()->FindComponentByClass<UDialogHandler>())
	{
		DialogHandler->Respond(Response);
	}
}
