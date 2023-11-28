// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/UI/DialogOptionButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CaravanGame/Player/CaravanPlayerController.h"
#include "CaravanGame/Dialog/DialogHandler.h"

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
