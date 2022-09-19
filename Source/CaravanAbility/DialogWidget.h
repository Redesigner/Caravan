// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	bool GetIsCompleted() const;

private:
	void PrewrapText(FString& Text) const;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UTextBlock* TextBox;

	FTimerHandle UpdateTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float SecondsPerCharacter = 0.01f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FString DesiredText;
	FString DisplayedText;

	uint8 CurrentIndex = 0;

	bool bIsCompleted = true;
};
