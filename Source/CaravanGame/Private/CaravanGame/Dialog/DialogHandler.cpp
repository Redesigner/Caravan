// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/Dialog/DialogHandler.h"

#include "CaravanGame/Character/CharacterBase.h"
#include "CaravanGame/Player/CaravanPlayerController.h"


UDialogHandler::UDialogHandler(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}


void UDialogHandler::QueueDialog(FName DialogId)
{
	if (DialogTable)
	{
		FString ContextString;
		if (FDialogInstance* Dialog = DialogTable->FindRow<FDialogInstance>(DialogId, ContextString))
		{
			QueueDialog(*Dialog);
		}
		else
		{
			// TODO: Separate log category
			UE_LOG(LogTemp, Warning, TEXT("Attempting to have the player load dialog '%s' from table '%s' failed -- there is no row with that name."),
				*DialogTable->GetName(), *DialogId.ToString())
		}
	}
}


void UDialogHandler::Respond(FString& Response)
{
	UE_LOG(LogTemp, Display, TEXT("Player clicked dialog option :'%s'"), *Response)
	if (Target.GetObject() && CharacterOwner.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("Sending Response to '%s'"), *Target.GetObject()->GetName())
	}
}


void UDialogHandler::QueueDialog(FDialogInstance& DialogInstance)
{
	UE_LOG(LogTemp, Display, TEXT("Dialog text added to queue: '%s'"), *DialogInstance.Text);
	DialogQueue.Enqueue(DialogInstance);
	if (!bDisplayingDialog)
	{
		if (DialogDisplayWidget)
		{
			ShowWidget();
			DisplayDialog(DialogInstance, true);
		}
	}
	else
	{
		FDialogInstance& CurrentDialog = *DialogQueue.Peek();
		if (CurrentDialog.bCanRespond)
		{
			AdvanceDialog();
		}
	}
}


void UDialogHandler::DisplayDialog(FDialogInstance& DialogInstance, bool bFadeIn)
{
	if (DialogInstance.bCanRespond)
	{
		DialogDisplayWidget->SetDialogWithOptions(DialogInstance.Text, DialogInstance.Responses, bFadeIn);
	}
	else
	{
		DialogDisplayWidget->SetDialog(DialogInstance.Text, bFadeIn);
		if (bFadeIn && DialogInstance.bCanAdvance)
		{
			FString ContextString;
			QueueDialog(*DialogTable->FindRow<FDialogInstance>(DialogInstance.NextDialog, ContextString));
		}
	}
}


void UDialogHandler::ShowWidget()
{
	if (ACaravanPlayerController* Controller = Cast<ACaravanPlayerController>(GetOwner()))
	{
		Controller->ShowCursor();
		if (CharacterOwner.IsValid())
		{
			CharacterOwner->DialogStart();
		}
		DialogDisplayWidget->SetIsEnabled(true);
		DialogDisplayWidget->SetVisibility(ESlateVisibility::Visible);
		bDisplayingDialog = true;
	}
}

void UDialogHandler::HideWidget()
{
	if (ACaravanPlayerController* Controller = Cast<ACaravanPlayerController>(GetOwner()))
	{
		Controller->HideCursor();
		if (CharacterOwner.IsValid())
		{
			CharacterOwner->DialogEnd();
		}
		DialogDisplayWidget->SetIsEnabled(false);
		DialogDisplayWidget->SetVisibility(ESlateVisibility::Hidden);
		bDisplayingDialog = false;
	}
}


void UDialogHandler::SetDialogTable(UDataTable* Table)
{
	DialogTable = Table;
}

void UDialogHandler::AttachWidget(UDialogWidget* Widget)
{
	DialogDisplayWidget = Widget;
}

void UDialogHandler::SetOwningCharacter(ACharacterBase* Character)
{
	CharacterOwner = Character;
}

void UDialogHandler::SetTarget(TScriptInterface<IInteractableInterface> Character)
{
	Target = Character;
}

bool UDialogHandler::IsActive() const
{
	return bDisplayingDialog;
}

void UDialogHandler::AdvanceDialog()
{
	if (DialogDisplayWidget)
	{
		if (DialogDisplayWidget->GetIsCompleted())
		{
			FDialogInstance NextDialog;
			DialogQueue.Dequeue(NextDialog);
			if (DialogQueue.IsEmpty())
			{
				if (!NextDialog.bCanRespond)
				{
					// If we don't have to respond, we can just hide the widget
					// otherwise, we wait
					HideWidget();
				}
				else
				{
					// For right now, we have to put this dialog back in the queue, otherwise,
					// the player can skip the response.
					// Not Dequeuing the dialog would cause IsEmpty() to always return true
					DialogQueue.Enqueue(NextDialog);
				}
			}
			else
			{
				DialogQueue.Peek(NextDialog);
				DisplayDialog(NextDialog, true);
			}
		}
		else
		{
			FDialogInstance& Dialog = *DialogQueue.Peek();
			DisplayDialog(Dialog, false);
		}
	}
}

