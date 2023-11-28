// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DialogOptionButton.generated.h"


UCLASS()
class CARAVANGAME_API UDialogOptionButton : public UUserWidget
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