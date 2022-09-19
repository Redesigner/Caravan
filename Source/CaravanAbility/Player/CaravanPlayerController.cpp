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

void ACaravanPlayerController::ReceiveDialogInteraction(AActor* SourceNPC, FName DialogID)
{
	// TODO: Process dialog and maybe use a different class to contain the next interactions, variables, etc.
	// Lookup dialog that has this tag.
	// DisplayText(Dialog);
	if (!DialogTable)
	{
		return;
	}
	FString ContextString;
	CurrentDialog = DialogTable->FindRow<FDialogInstance>(DialogID, ContextString);
	if (CurrentDialog)
	{
		DisplayText(FText::FromString(CurrentDialog->Text) );
	}
}

void ACaravanPlayerController::Interact()
{
	if (!bIsDisplayingText)
	{
		PlayerCharacter->Interact();
	}
	else
	{
		AdvanceText();
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
	if (PlayerCharacter.IsValid())
	{
		PlayerCharacter->PauseMovement();
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
	if (PlayerCharacter.IsValid())
	{
		PlayerCharacter->UnpauseMovement();
	}
}

void ACaravanPlayerController::AdvanceText()
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
