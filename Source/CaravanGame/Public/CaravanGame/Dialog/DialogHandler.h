// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CaravanGame/UI/DialogWidget.h"
#include "CaravanGame/Dialog/DialogInstance.h"
#include "Containers/Queue.h" 
#include "DialogHandler.generated.h"

class ACharacterBase;
class UDialogComponent;

UCLASS()
class CARAVANGAME_API UDialogHandler : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UDialogHandler(const FObjectInitializer& ObjectInitializer);

	void SetDialogTable(UDataTable* Table);

	void AttachWidget(UDialogWidget* Widget);
	void SetOwningCharacter(ACharacterBase* Character);
	void SetTarget(UDialogComponent* TargetComponent);

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
	TWeakObjectPtr<UDialogComponent> Target;
	TWeakObjectPtr<ACharacterBase> CharacterOwner;

	UPROPERTY()
	UDialogWidget* DialogDisplayWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Dialog, meta = (AllowPrivateAccess = "true"))
	UDataTable* DialogTable;

	bool bDisplayingDialog = false;
};
