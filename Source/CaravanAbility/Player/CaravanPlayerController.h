// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" 
#include "CaravanAbility/DialogWidget.h"
#include "CaravanAbility/Dialog/DialogInstance.h"
#include "GameplayTagContainer.h" 

#include "CaravanPlayerController.generated.h"

/**
 * 
 */
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
	UFUNCTION(BlueprintCallable)
	void ReceiveDialogInteraction(AActor* SourceNPC, FName DialogID);

	UFUNCTION(BlueprintCallable)
	void Interact();

private:
	void DisplayText(FText Text);

	void HideText();

	void AdvanceText();

	TWeakObjectPtr<class ACharacterBase> PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDialogWidget> DialogWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Dialog, meta = (AllowPrivateAccess = "true"))
	UDataTable* DialogTable;

	FDialogInstance* CurrentDialog;

	bool bIsDisplayingText = false;
	
};
