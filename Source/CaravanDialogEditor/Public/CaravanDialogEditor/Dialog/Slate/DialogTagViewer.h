// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h" 
#include "Widgets/Views/STreeView.h"
#include "PropertyHandle.h" 

#include "CaravanGame/Dialog/Tags/DialogTagHandle.h"

/**
 * 
 */
class UDialogTagManager;

struct FDialogTagNode
{
	FDialogTagNode(FDialogTag* Tag) : Tag(Tag) {};
	FDialogTag* Tag;
	TArray<TSharedPtr<FDialogTagNode>> Children;
};

class CARAVANDIALOGEDITOR_API SDialogTagViewer : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SDialogTagViewer){} 
		SLATE_ATTRIBUTE( TSharedPtr<IPropertyHandle>, PropertyHandle )
		SLATE_ATTRIBUTE( UDialogTagManager*, DialogTagManager )
	SLATE_END_ARGS()
public:
	using NodeType = TSharedPtr<FDialogTagNode>;
	void Construct(const FArguments& InArgs);

	TSharedRef<class ITableRow> HandleGenerateRow(NodeType DialogTag, const TSharedRef<STableViewBase>& TableViewBase);
	void HandleGetChildren(NodeType InItem, TArray<NodeType>& OutChildren);
	void HandleSetExpansionRecursive(NodeType InTreeNode, bool bIsItemExpanded);
	void HandleButtonClick(NodeType NodeClicked);

	void HandleAddChildButtonClick(NodeType NodeClicked);

	void SetDisplayText(FText& Text);


private:
	void CreateChildren(NodeType ParentNode);

	UDialogTagManager* DialogTagManager;

	TSharedPtr<STreeView<NodeType>> TreeView;
	TSharedPtr<SComboButton> BaseButton;
	TSharedPtr<SHeaderRow> HeaderRow;
	TSharedPtr<IPropertyHandle> PropertyHandle;
	TArray<NodeType> DialogTags;
	FText PropertyDisplayText;
};
