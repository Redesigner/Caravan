// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CaravanAbility/UI/DialogOptionButton.h"
#include "DialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANABILITY_API UDialogWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void SetDialog(FString Text, bool bFadeIn = true);

	UFUNCTION(BlueprintCallable)
	void SetDialogWithOptions(FString Text, TArray<FString> Options, bool bFadeIn = true);

	UFUNCTION(BlueprintCallable)
	bool GetIsCompleted() const;

	DECLARE_DELEGATE_OneParam(OptionClicked, FString);
	OptionClicked OnOptionClicked;

private:
	void PrewrapText(FString& Text) const;

	void ClearOptions();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UTextBlock* TextBox = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UVerticalBox* OptionList = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDialogOptionButton> OptionButtonClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UDialogOptionButton*> OptionButtons;

	FTimerHandle UpdateTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float SecondsPerCharacter = 0.01f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FString DesiredText;
	FString DisplayedText;

	uint8 CurrentIndex = 0;

	bool bIsCompleted = true;
};
