// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogWidget.h"

#include "Fonts/FontMeasure.h"


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
		BreakLocation = FontMeasure->FindCharacterIndexAtOffset(FText::FromString(Text), TextBox->Font, MaxWidth);
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
