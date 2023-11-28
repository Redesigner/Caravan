// Copyright (c) 2023 Stephen Melnick


#include "CaravanDialogEditor/Dialog/Slate/DialogTagNodeViewer.h"

#include "Widgets/Views/STreeView.h"
#include "Widgets/Input/STextEntryPopup.h" 

#include "CaravanGame/Dialog/Tags/DialogTag.h"
#include "CaravanGame/Dialog/Tags/DialogTagManager.h"
#include "CaravanGame/Dialog/DialogSubsystem.h"

#include "CaravanDialogEditor/Dialog/Slate/DialogTagViewer.h"

using NodeType = TSharedPtr<FDialogTagNode>;


void SDialogTagNodeViewer::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Node = InArgs._DialogTagNode.Get();

	EditableTextBox = SNew(SEditableText)
		.Text(MakeAttributeLambda([=] { return FText::FromName(Node->Tag->Tag); }))
		.OnTextCommitted(this, &SDialogTagNodeViewer::HandleNameCommitted);
	NewTagPopup = SNew(STextEntryPopup)
		.DefaultText(PopupText)
		.OnTextCommitted(this, &SDialogTagNodeViewer::HandleNewNameCommitted)
		.Label(FText::FromString(TEXT("New Child Tag:")));
	AddChildAnchor = SNew(SMenuAnchor)
	.Content()
	[
		SNew(SButton)
		.OnClicked(TDelegate<FReply(void)>::CreateSP(this, &SDialogTagNodeViewer::HandleOnClicked))
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("+")))
		]
	]
	.MenuContent(NewTagPopup);

	TSharedRef<SBox> InnerBox = SNew(SBox)
		.Content()
		[
			EditableTextBox.ToSharedRef()
		];

	Super::FArguments Args;
	Args
		.Padding(10)
		.Content()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				InnerBox
			]
			+ SHorizontalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Right)
			[
				AddChildAnchor.ToSharedRef()
			]
		];
	Super::Construct(Args, InOwnerTableView);
}

void SDialogTagNodeViewer::HandleNameCommitted(const FText& CommittedText, ETextCommit::Type)
{
	FName NewName = FName(CommittedText.ToString());
	if (!Node->Tag)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to rename an invalid dialog tag."));
		return;
	}
	
	UDialogSubsystem* DialogSubsystem = GEngine->GetEngineSubsystem<UDialogSubsystem>();
	DialogSubsystem->GetTagManager()->RenameTag(Node->Tag, NewName);
	EditableTextBox->SetText(FText::FromName(NewName));
}


FReply SDialogTagNodeViewer::HandleOnClicked()
{
	if (!AddChildAnchor->IsOpen())
	{
		AddChildAnchor->SetIsOpen(true);
	}
	return FReply::Handled();
}


void SDialogTagNodeViewer::HandleNewNameCommitted(const FText& CommittedText, ETextCommit::Type)
{
	if (!Node->Tag)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to add a new child to an invalid dialog tag."));
		AddChildAnchor->SetIsOpen(false);
		return;
	}
	Node->Tag->AddChild(FName(CommittedText.ToString()));
	AddChildAnchor->SetIsOpen(false);
	PopupText = FText::FromString(TEXT(""));	
}
