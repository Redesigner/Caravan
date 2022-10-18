// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogTagViewer.h"

#include "PropertyEditorModule.h" 
#include "PropertyEditor/Public/DetailLayoutBuilder.h"
#include "PropertyEditor/Public/DetailWidgetRow.h"
#include "PropertyEditor/Public/PropertyHandle.h"

#include "SlateBasics.h"

#include "IDetailChildrenBuilder.h"

#include "Widgets/Input/SComboButton.h" 
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Views/STreeView.h" 
#include "Widgets/Views/STableRow.h"

#include "CaravanAbility\Dialog\Tags\DialogTag.h"
#include "CaravanAbility\Dialog\Tags\DialogTagManager.h"

#include "CaravanAbilityDialogEditor\Dialog\Slate\DialogTagNodeViewer.h"


void SDialogTagViewer::Construct(const FArguments& InArgs)
{

	HeaderRow = SNew(SHeaderRow);
	PropertyHandle = InArgs._PropertyHandle.Get();
	DialogTagManager = InArgs._DialogTagManager.Get();
	
	void* ValueData;
	PropertyHandle->GetValueData(ValueData);
	FDialogTagHandle* DialogTagHandle = reinterpret_cast<FDialogTagHandle*>(ValueData);
	const FDialogTag* PropertyDialogTag = DialogTagHandle->GetTag();
	if (PropertyDialogTag)
	{
		PropertyDisplayText = FText::FromString(PropertyDialogTag->GetFullName());
	}

	for (FDialogTag* DialogTag : DialogTagManager->GetRootTags())
	{
		NodeType NewNode = MakeShared<FDialogTagNode>( FDialogTagNode(DialogTag) );
		CreateChildren(NewNode);
		DialogTags.Add(NewNode);
	}
	BaseButton = SNew(SComboButton)
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(MakeAttributeLambda([=] { return PropertyDisplayText; }))
		];

	TreeView = SNew(STreeView<NodeType>)
		.TreeItemsSource(&DialogTags)
		.OnGenerateRow(this, &SDialogTagViewer::HandleGenerateRow)
		.OnGetChildren(this, &SDialogTagViewer::HandleGetChildren)
		.OnSetExpansionRecursive(this, &SDialogTagViewer::HandleSetExpansionRecursive)
		.OnMouseButtonClick(this, &SDialogTagViewer::HandleButtonClick)
		.HeaderRow(HeaderRow);

	BaseButton->SetMenuContent(
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		[
			SNew(SBox)
			.MinDesiredWidth(200)
			.MinDesiredHeight(200)
			.Content()
			[
				TreeView.ToSharedRef()
			]
		]);

	TSharedRef<SBox> Box = SNew(SBox);
	// Box->SetMinDesiredWidth(400);
	Box->SetContent(SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			BaseButton.ToSharedRef()
		]);

	ChildSlot
		[
			Box
		];
}

TSharedRef<ITableRow> SDialogTagViewer::HandleGenerateRow(NodeType DialogTag, const TSharedRef<STableViewBase>& TableViewBase)
{
	return SNew(SDialogTagNodeViewer, TableViewBase).DialogTagNode(DialogTag);
}

void SDialogTagViewer::HandleGetChildren(NodeType InItem, TArray<NodeType>& OutChildren)
{
	OutChildren = InItem->Children;
}

void SDialogTagViewer::HandleSetExpansionRecursive(NodeType InItem, bool bIsItemExpanded)
{

	TArray<NodeType> Tags = InItem->Children;
	if (Tags.Num())
	{
		TreeView->SetItemExpansion(InItem, bIsItemExpanded);
		const bool bShouldSaveState = true;

		for (NodeType Child : Tags)
		{
			TreeView->SetItemExpansion(Child, bIsItemExpanded);
		}
	}
}


void SDialogTagViewer::HandleButtonClick(NodeType NodeClicked)
{
	TreeView->SetSelection(NodeClicked);

	FDialogTagHandle EmptyHandle;
	FDialogTagHandle NewHandle = DialogTagManager->GetTagHandle(NodeClicked->Tag);
	FString HandleAsString;
	FDialogTagHandle::StaticStruct()->ExportText(HandleAsString, &NewHandle, &EmptyHandle, /*OwnerObject*/nullptr, PPF_None, /*ExportRootScope*/nullptr);
	PropertyHandle->SetValueFromFormattedString(HandleAsString);

	// PropertyHandle->NotifyPreChange();

	// void* HandlePtr;
	// PropertyHandle->GetValueData(HandlePtr);
	// FDialogTagHandle* TagHandle = reinterpret_cast<FDialogTagHandle*>(HandlePtr);
	// *TagHandle = DialogTagManager->GetTagHandle(NodeClicked->Tag);


	PropertyDisplayText = FText::FromString(NodeClicked->Tag->GetFullName());
	TreeView->RequestListRefresh();

	// PropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
	// PropertyHandle->NotifyFinishedChangingProperties();

	BaseButton->SetIsOpen(false);
}

void SDialogTagViewer::HandleAddChildButtonClick(NodeType NodeClicked)
{
}

void SDialogTagViewer::SetDisplayText(FText& Text)
{
	// PropertyDisplayText = Text;
}


void SDialogTagViewer::CreateChildren(NodeType ParentNode)
{
	if (ParentNode->Tag->HasChildren())
	{
		for (int32 Index : ParentNode->Tag->Children)
		{
			FDialogTag* ChildTag = &DialogTagManager->Tags[Index];
			NodeType ChildNode = MakeShared<FDialogTagNode>(ChildTag);
			ParentNode->Children.Add(ChildNode);
			CreateChildren(ChildNode);
		}
	}
}
