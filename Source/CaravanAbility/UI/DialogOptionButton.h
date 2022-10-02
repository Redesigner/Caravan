// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DialogOptionButton.generated.h"


UCLASS()
class CARAVANABILITY_API UDialogOptionButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UTextBlock* OptionText;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (BindWidget, AllowPrivateAccess = "true") )
	class UButton* OptionButton;

public:
	void SetText(FString Text);

	DECLARE_DELEGATE_OneParam(ButtonClicked, FString)
	ButtonClicked OnButtonClicked;

private:
	UFUNCTION()
	void SendClick();

	FString Response;
};