// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/SCompoundWidget.h" 
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/ITableRow.h"

#include "CaravanAbilityDialogEditor/Dialog/Slate/DialogTagViewer.h"


/**
 * 
 */
using NodeType = TSharedPtr<FDialogTagNode>;

class CARAVANABILITYDIALOGEDITOR_API SDialogTagNodeViewer : public STableRow< TSharedPtr<FString> >
{
	SLATE_BEGIN_ARGS(SDialogTagNodeViewer) {}
		SLATE_ATTRIBUTE(NodeType, DialogTagNode)
	SLATE_END_ARGS()

	using Super = STableRow< TSharedPtr<FString> >;

public:

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);

	void HandleNameCommitted(const FText& CommittedText, ETextCommit::Type);

	FReply HandleOnClicked();

	void HandleNewNameCommitted(const FText& CommittedText, ETextCommit::Type);

private:
	NodeType Node;
	TSharedPtr<STableRow< TSharedPtr<FString> >> TableRow;
	TSharedPtr<SEditableText> EditableTextBox;
	TSharedPtr<SMenuAnchor> AddChildAnchor;
	TSharedPtr<STextEntryPopup> NewTagPopup;

	FText PopupText;
};
