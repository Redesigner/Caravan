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
class UInventoryUserWidget;

UCLASS()
class CARAVANABILITY_API ACaravanPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACaravanPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY()
	UDialogWidget* DialogDisplayWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UInventoryUserWidget* InventoryWidget;

public:
	void ShowCursor();

	void HideCursor();

	UFUNCTION(BlueprintCallable)
	void Interact();

private:
	TWeakObjectPtr<ACharacterBase> PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDialogWidget> DialogWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryUserWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	class UDialogHandler* DialogHandler;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	class UInventoryContainer* InventoryContainer;
};
