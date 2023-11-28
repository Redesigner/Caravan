// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

#include "CaravanGame/Dialog/Tags/DialogTagHandle.h"

#include "DialogTag.generated.h"

/**
 * 
 */

class UDialogTagManager;

USTRUCT(BlueprintType)
struct CARAVANGAME_API FDialogTag
{

	GENERATED_BODY()

	bool operator ==(const FDialogTag& a) const;

	FDialogTag();

	FDialogTag(FName& Tag);

	FDialogTag(FName& Tag, int32 Parent);

	UPROPERTY()
	FName Tag;

private:
	bool HasChildren() const;

	void AddChild(FName NewTagName);

	FName GetName() const;

	FString GetFullName() const;

	void SetName(FName& Name);

	void RenameInstances(const FString& NewFullName);

	int32 InternalIndex;

	int32 Parent;

	TArray<int32> Children;

	UDialogTagManager* Owner = nullptr;

	friend class UDialogTagManager;
	friend class SDialogTagViewer;
	friend class SDialogTagNodeViewer;
};