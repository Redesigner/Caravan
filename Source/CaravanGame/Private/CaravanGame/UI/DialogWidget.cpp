// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/UI/DialogWidget.h"

#include "Fonts/FontMeasure.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/PanelWidget.h" 
#include "Components/TextBlock.h" 


void UDialogWidget::SetDialog(FString Text, bool bFadeIn)
{
	if (!bIsCompleted)
	{
		if (!bFadeIn)
		{
			PrewrapText(Text);
			TextBox->SetText(FText::FromString(Text));
			bIsCompleted = true;
		}
	}
	if (bFadeIn)
	{
		ClearOptions();
		PrewrapText(Text);
		DesiredText = Text;
		DisplayedText = "";
		TextBox->SetText(FText::FromString(DisplayedText));
		CurrentIndex = 0;
		bIsCompleted = false;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(UpdateTimerHandle, [this]()
				{
					if (CurrentIndex < DesiredText.Len() && !bIsCompleted)
					{
						DisplayedText += DesiredText[CurrentIndex++];
						TextBox->SetText(FText::FromString(DisplayedText));
					}
					else
					{
						bIsCompleted = true;
					}
				},
				SecondsPerCharacter, true);
		}
	}
}

void UDialogWidget::SetDialogWithOptions(FString Text, TArray<FString> Options, bool bFadeIn)
{
	SetDialog(Text, bFadeIn);
	for (FString& Option : Options)
	{
		if (!OptionButtonClass->StaticClass())
		{
			UE_LOG(LogTemp, Warning, TEXT("Tried to create buttons for dialog, but the widget class was null."));
			return;
		}
		UDialogOptionButton* NewButton = CreateWidget<UDialogOptionButton>(this, OptionButtonClass);
		OptionList->AddChildToVerticalBox(NewButton);
		NewButton->SetText(Option);
	}
}

bool UDialogWidget::GetIsCompleted() const
{
	return bIsCompleted;
}


void UDialogWidget::PrewrapText(FString& Text) const
{
	const TSharedRef<FSlateFontMeasure>& FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	int MaxWidth = 650;
	// int MaxWidth = TextBox->GetDesiredSize().X - 50;
	int32 BreakLocation;
	for (int i = 0; i < 5; i++)
	{
		BreakLocation = FontMeasure->FindCharacterIndexAtOffset(FText::FromString(Text), TextBox->GetFont(), MaxWidth);
		if (BreakLocation >= Text.Len())
		{
			return;
		}
		while (BreakLocation > 0 && !FText::IsWhitespace(Text[BreakLocation]))
		{
			BreakLocation--;
		}
		Text[BreakLocation] = '\n';
	}
}

void UDialogWidget::ClearOptions()
{
	if (OptionList->HasAnyChildren())
	{
		for (UWidget* Widget : OptionList->GetAllChildren())
		{
			OptionList->RemoveChild(Widget);
		}
	}
}
