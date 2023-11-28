// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/Dialog/Tags/DialogTagHandle.h"

#include "CaravanGame/Dialog/DialogResponseInterface.h"
#include "CaravanGame/Dialog/DialogSubsystem.h"
#include "CaravanGame/Dialog/Tags/DialogTagManager.h"


FDialogTagHandle::FDialogTagHandle()
{
	UE_LOG(LogTemp, Warning, TEXT("Loaded tag with name: '%s', but didn't load pointer... nulling value..."), *TagName);
	OriginalTag = nullptr;
}

FDialogTagHandle::FDialogTagHandle(FDialogTag* Tag)
{
	OriginalTag = Tag;
}

bool FDialogTagHandle::Serialize(FArchive& Ar)
{
	Ar << TagName;
	return true;
}

FString FDialogTagHandle::GetFullName() const
{
	return TagName;
}

void FDialogTagHandle::UpdateInternalTag()
{
	// Our dialog subsystem hasn't loaded yet, so we can't get the internal tag
	if (!GEngine)
	{
		return;
	}
	if (!OriginalTag)
	{
		return;
	}
	FString NameToFind = TagName;
	UDialogSubsystem* DialogSubsystem = GEngine->GetEngineSubsystem<UDialogSubsystem>();
	UDialogTagManager* TagManager = DialogSubsystem->GetTagManager();
	FDialogTag* NewTag = TagManager->FindTagByName(NameToFind);
	UE_LOG(LogTemp, Warning, TEXT("Loaded tag with name: '%s', attempting to find matching tag in master record."), *TagName);
	if (!NewTag)
	{
		OriginalTag = TagManager->GetRootTags()[0];
		UE_LOG(LogTemp, Warning, TEXT("Unable to find dialog tag with name '%s', assigning value to first tag found."), *TagName);
	}
}


void FDialogTagHandle::RenameTag(const FString& NewName)
{
	TagName = NewName;
}

const FDialogTag* FDialogTagHandle::GetTag()
{
	UpdateInternalTag();
	return OriginalTag;
}

FArchive& operator<<(FArchive& Ar, FDialogTagHandle& TagHandle)
{
	Ar << TagHandle.TagName;
	return Ar;
}
