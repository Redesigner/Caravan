// Fill out your copyright notice in the Description page of Project Settings.


#include "CaravanPlayerController.h"

#include "CaravanAbility\Character\CharacterBase.h"

#include "Components/Widget.h" 

ACaravanPlayerController::ACaravanPlayerController()
{
}

void ACaravanPlayerController::BeginPlay()
{
	if (DialogWidgetClass && IsLocalPlayerController() )
	{
		DialogDisplayWidget = CreateWidget<UDialogWidget>(this, DialogWidgetClass, TEXT("Dialog display"));
		DialogDisplayWidget->AddToPlayerScreen();
		HideText();
	}

	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		PlayerCharacter = CharacterBase;
	}
}

void ACaravanPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ACaravanPlayerController::Interact);
}

void ACaravanPlayerController::ReceiveDialog(FName Dialog, TWeakObjectPtr<ACharacterBase> CharacterReceiver)
{
	if (bIsDisplayingText)
	{
		// Right now, displayed dialog cannot be overwritten. Maybe this will change?
		return;
	}
	if (!DialogTable)
	{
		return;
	}
	FString ContextString;
	CurrentDialog = DialogTable->FindRow<FDialogInstance>(Dialog, ContextString);
	if (CurrentDialog)
	{
		if (CharacterReceiver.IsValid())
		{
			CurrentDialogOwner = CharacterReceiver;
		}
		DisplayText(FText::FromString(CurrentDialog->Text));
	}
}

void ACaravanPlayerController::Interact()
{
	// Normally, we'll just forward this event to the character. Since
	// the controller takes responsibility for displaying dialog on screen,
	// it gets advanced here.
	if (!bIsDisplayingText)
	{
		PlayerCharacter->Interact();
	}
	else
	{
		TryAdvanceText();
	}
}

void ACaravanPlayerController::DisplayText(FText Text)
{
	// This shouldn't be called by a non-local player controller, but we just want to make sure...
	if (DialogDisplayWidget && IsLocalPlayerController())
	{
		DialogDisplayWidget->SetDialog(Text.ToString());
		DialogDisplayWidget->SetIsEnabled(true);
		DialogDisplayWidget->SetVisibility(ESlateVisibility::Visible);
		bIsDisplayingText = true;
	}
	if (CurrentDialogOwner.IsValid())
	{
		CurrentDialogOwner->DialogStart();
	}
}

void ACaravanPlayerController::HideText()
{
	if (DialogDisplayWidget && IsLocalPlayerController())
	{
		DialogDisplayWidget->SetIsEnabled(false);
		DialogDisplayWidget->SetVisibility(ESlateVisibility::Hidden);
		bIsDisplayingText = false;
	}
	if (CurrentDialogOwner.IsValid())
	{
		// We're done looking at the dialog, so make sure to let our character/pawn know
		CurrentDialogOwner->DialogEnd();
	}
}


void ACaravanPlayerController::TryAdvanceText()
{
	if (bIsDisplayingText)
	{
		if (!DialogDisplayWidget->GetIsCompleted())
		{
			if (CurrentDialog)
			{
				DialogDisplayWidget->SetDialog(CurrentDialog->Text, false);
			}
		}
		else if (CurrentDialog->bCanAdvance)
		{
			FString ContextString;
			CurrentDialog = DialogTable->FindRow<FDialogInstance>(CurrentDialog->NextDialog, ContextString);
			if (CurrentDialog)
			{
				DialogDisplayWidget->SetDialog(CurrentDialog->Text);
			}
		}
		else
		{
			HideText();
		}
	}
}
