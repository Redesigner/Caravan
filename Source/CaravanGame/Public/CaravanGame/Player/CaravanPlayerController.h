// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" 
#include "GameplayTagContainer.h" 

#include "CaravanGame/UI/DialogWidget.h"
#include "CaravanGame/Dialog/DialogInstance.h"

#include "CaravanPlayerController.generated.h"

/**
 * 
 */
class ACharacterBase;
class UInventoryUserWidget;

UCLASS()
class CARAVANGAME_API ACaravanPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACaravanPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;


public:
	void ShowCursor();

	void HideCursor();

	UFUNCTION(BlueprintCallable)
	void Interact();

private:
	TWeakObjectPtr<ACharacterBase> PlayerCharacter;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDialogWidget> DialogWidgetClass;

	UPROPERTY()
	UDialogWidget* DialogDisplayWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	class UDialogHandler* DialogHandler;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryUserWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UInventoryUserWidget* InventoryWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Interface, meta = (AllowPrivateAccess = "true"))
	class UInventoryContainer* InventoryContainer;
};
