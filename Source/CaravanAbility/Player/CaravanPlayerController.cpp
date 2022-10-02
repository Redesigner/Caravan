// Fill out your copyright notice in the Description page of Project Settings.


#include "CaravanPlayerController.h"

#include "CaravanAbility\Character\CharacterBase.h"
#include "CaravanAbility\Dialog\DialogHandler.h"

#include "Components/Widget.h" 

ACaravanPlayerController::ACaravanPlayerController()
{
	DialogHandler = CreateDefaultSubobject<UDialogHandler>(TEXT("Dialog Handler"));
}

void ACaravanPlayerController::BeginPlay()
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetPawn()))
	{
		PlayerCharacter = CharacterBase;
	}

	if (DialogWidgetClass && IsLocalPlayerController() )
	{
		DialogDisplayWidget = CreateWidget<UDialogWidget>(this, DialogWidgetClass, TEXT("Dialog display"));
		DialogDisplayWidget->AddToPlayerScreen();
		DialogDisplayWidget->SetVisibility(ESlateVisibility::Hidden);
		DialogHandler->AttachWidget(DialogDisplayWidget);
		DialogHandler->SetOwningCharacter(PlayerCharacter.Get() );
	}
}

void ACaravanPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ACaravanPlayerController::Interact);
}


void ACaravanPlayerController::Interact()
{
	// Normally, we'll just forward this event to the character. Since
	// the controller takes responsibility for displaying dialog on screen,
	// it gets advanced here.
	if (DialogHandler && DialogHandler->IsActive())
	{
		DialogHandler->AdvanceDialog();
	}
	else
	{
		PlayerCharacter->Interact();
	}
}


void ACaravanPlayerController::ShowCursor()
{
	SetShowMouseCursor(true);
	int x, y;
	GetViewportSize(x, y);
	SetMouseLocation(x / 2, y - 100);
	SetInputMode(FInputModeGameAndUI());
}


void ACaravanPlayerController::HideCursor()
{
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
}
