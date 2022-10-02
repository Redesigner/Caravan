// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CaravanAbility/UI/DialogWidget.h"
#include "CaravanAbility/Dialog/DialogInstance.h"
#include "Containers/Queue.h" 
#include "DialogHandler.generated.h"

class ACharacterBase;

UCLASS()
class CARAVANABILITY_API UDialogHandler : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UDialogHandler(const FObjectInitializer& ObjectInitializer);

	void SetDialogTable(UDataTable* Table);
	void AttachWidget(UDialogWidget* Widget);
	void SetOwningCharacter(ACharacterBase* Character);
	void SetTargetCharacter(ACharacterBase* Character);

	bool IsActive() const;

	void AdvanceDialog();

	void QueueDialog(FName DialogId);

	void Respond(FString& Response);


private:
	void QueueDialog(FDialogInstance& DialogInstance);

	void DisplayDialog(FDialogInstance& DialogInstance, bool bFadeIn);

	void ShowWidget();

	void HideWidget();

	// TODO: Use a custom container so that Dialog Instances can be stored as reference?
	TQueue<FDialogInstance> DialogQueue;
	TWeakObjectPtr<ACharacterBase> Target;
	TWeakObjectPtr<ACharacterBase> CharacterOwner;

	UPROPERTY()
	UDialogWidget* DialogDisplayWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Dialog, meta = (AllowPrivateAccess = "true"))
	UDataTable* DialogTable;

	bool bDisplayingDialog = false;
};
