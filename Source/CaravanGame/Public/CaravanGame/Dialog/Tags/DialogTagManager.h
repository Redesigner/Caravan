// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

#include "CaravanGame/Dialog/DialogResponseInterface.h"
#include "CaravanGame/Dialog/Tags/DialogTagHandle.h"
#include "CaravanGame/Dialog/Tags/DialogTag.h"

#include "JsonDomBuilder.h" 


#include "DialogTagManager.generated.h"

/**
 * 
 */
UCLASS()
class CARAVANGAME_API UDialogTagManager : public UObject
{
	GENERATED_BODY()

public:

	UDialogTagManager(const FObjectInitializer& ObjectInitializer);
	~UDialogTagManager();

	void LoadFile();

	void SaveFile();

	FDialogTag* FindTagByName(const FString& TagFullName);

	FDialogTagHandle GetTagHandle(const FString& TagFullName);

	FDialogTagHandle GetTagHandle(FDialogTag* Tag);

	FDialogTag& CreateTag(FString& TagFullName);

	// @TODO: Don't regenerate all tag names, or even all parent names
	TArray<FString> GetAllFullTagNames();

	void RenameTag(FDialogTag* Tag, FName& NewName);

	TArray<FDialogTag*> GetChildren(const FDialogTag* Tag);

	TArray<FDialogTag* > GetRootTags();


	TArray<FDialogTag> Tags;


private:
	void AppendChildTags(FJsonDomBuilder::FObject* Parent, FDialogTag& Tag);

	void GenerateTagsFromChildren(TSharedPtr<FJsonObject> Parent, int32 ParentIndex);

	int32 CreateTag(FName& Tag);

	int32 CreateTag(FName& Tag, int32 Parent);


	const FString GetFileName();


	FDialogTag RootTag;

	bool bIsLoaded = false;

	friend FDialogTag;
};
