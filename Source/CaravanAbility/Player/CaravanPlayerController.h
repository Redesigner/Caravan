// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" 
#include "CaravanAbility/UI/DialogWidget.h"
#include "CaravanAbility/Dialog/DialogInstance.h"
#include "GameplayTagContainer.h" 

#include "CaravanPlayerController.generated.h"

/**
 * 
 */
class ACharacterBase;

UCLASS()
class CARAVANABILITY_API ACaravanPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACaravanPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY()
	UDialogWidget* DialogDisplayWidget;

public:
	void ReceiveDialog(FName Dialog, TWeakObjectPtr<ACharacterBase> CharacterReceiver);

	UFUNCTION(BlueprintCallable)
	void Interact();

private:
	void DisplayText(FText Text);
	void HideText();
	void TryAdvanceText();

	TWeakObjectPtr<ACharacterBase> PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDialogWidget> DialogWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Dialog, meta = (AllowPrivateAccess = "true"))
	UDataTable* DialogTable;

	FDialogInstance* CurrentDialog;
	TWeakObjectPtr<ACharacterBase> CurrentDialogOwner;

	bool bIsDisplayingText = false;
	
};
