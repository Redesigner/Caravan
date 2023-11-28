// Copyright (c) 2023 Stephen Melnick


#include "CaravanGame/Dialog/Tags/DialogTagManager.h"

#include "GenericPlatform/GenericPlatform.h" 
#include "Misc/FileHelper.h"
#include "Misc/Paths.h" 
#include "HAL/FileManager.h" 

#include "JsonDomBuilder.h" 
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "UObject/WeakFieldPtr.h" 

#include "CaravanGame/Dialog/Tags/DialogTagHandle.h"


const FString TAGFILENAME = TEXT("DialogTags.ini");

using CharType = FPlatformTypes::ANSICHAR;

using JsonValPtr = TSharedPtr<FJsonValue>;
using JsonObjPtr = TSharedPtr<FJsonObject>;


UDialogTagManager::UDialogTagManager(const FObjectInitializer& ObjectInitializer)
{
}

UDialogTagManager::~UDialogTagManager()
{
	// We want to make sure we save **before** GC tries to clean up the object...
	// Otherwise, in some cases, the GC will begin deleting tags -- causing corrupt files
	SaveFile();
}

int32 UDialogTagManager::CreateTag(FName& Tag)
{
	int32 TagIndex = Tags.Emplace(Tag, -1);
	Tags[TagIndex].Owner = this;
	Tags[TagIndex].InternalIndex = TagIndex;
	RootTag.Children.Add(TagIndex);
	return TagIndex;
}


int32 UDialogTagManager::CreateTag(FName& Tag, int32 Parent)
{
	if (!Tags.IsValidIndex(Parent))
	{
		return -1;
	}
	int32 TagIndex = Tags.Emplace(Tag, Parent);
	Tags[Parent].Children.Add(TagIndex);
	Tags[TagIndex].Owner = this;
	Tags[TagIndex].InternalIndex = TagIndex;
	return TagIndex;
}


FDialogTag& UDialogTagManager::CreateTag(FString& TagFullName)
{
	int32 StrIndex;
	if (TagFullName.FindLastChar('.', StrIndex))
	{
		FString TagParentName = TagFullName.Left(StrIndex);
		if (FDialogTag* ParentTag = FindTagByName(TagParentName))
		{
			FName TagName = FName(TagFullName.RightChop(StrIndex + 1));
			int32 TagIndex = CreateTag(TagName, Tags.Find(*ParentTag));
			FDialogTag& NewTag = Tags[TagIndex];
			NewTag.InternalIndex = TagIndex;
			return NewTag;
		}
		// Return an error here?
		return RootTag;
	}
	else
	{
		FName TagName = FName(TagFullName);
		int32 TagIndex = CreateTag(TagName);
		Tags[TagIndex].Owner = this;
		Tags[TagIndex].InternalIndex = TagIndex;
		return Tags[TagIndex];
	}
}

TArray<FString> UDialogTagManager::GetAllFullTagNames()
{
	TArray<FString> AllNames;
	for (FDialogTag& Tag : Tags)
	{
		AllNames.Add(Tag.GetFullName());
	}
	return AllNames;
}

void UDialogTagManager::RenameTag(FDialogTag* Tag, FName& NewName)
{
	FDialogTag TagToRename;
	int32 Index;
	if (Tags.Find(*Tag, Index))
	{
		FDialogTag& OriginalTag = Tags[Index];
		OriginalTag.SetName(NewName);
		// OriginalTag.RenameInstances(GetFullTagName(OriginalTag));
	}
}


TArray<FDialogTag*> UDialogTagManager::GetChildren(const FDialogTag* Tag)
{
	TArray<FDialogTag*> ChildTags;
	for (int32 ChildIndex : Tag->Children)
	{
		if (Tags.IsValidIndex(ChildIndex))
		{
			ChildTags.Add(&Tags[ChildIndex]);
		}
	}
	return ChildTags;
}

TArray<FDialogTag*> UDialogTagManager::GetRootTags()
{
	TArray<FDialogTag*> RootTags;
	for (int32 ChildIndex : RootTag.Children)
	{
		RootTags.Add(&Tags[ChildIndex]);
	}
	return RootTags;
}

FDialogTag* UDialogTagManager::FindTagByName(const FString& TagFullName)
{
	TArray<FString> TagHierarchy;
	TagFullName.ParseIntoArray(TagHierarchy, TEXT("."));
	FDialogTag* CurrentNodeTag = &RootTag;
	for (int i = 0; i < TagHierarchy.Num(); i++)
	{
		FString& TagLeafName = TagHierarchy[i];
		// Check all of our children to find the tag with this subname
		for (int32 TagIndex : CurrentNodeTag->Children)
		{
			if (Tags[TagIndex].Tag.ToString() == TagLeafName)
			{
				CurrentNodeTag = &Tags[TagIndex];
				if (i == TagHierarchy.Num() - 1)
				{
					return CurrentNodeTag;
				}
				break;
			}
		}
	}
	return nullptr;
}


FDialogTagHandle UDialogTagManager::GetTagHandle(const FString& TagFullName)
{
	// Redo this so we don't have to generate the name again
	return GetTagHandle(FindTagByName(TagFullName));
}

FDialogTagHandle UDialogTagManager::GetTagHandle(FDialogTag* Tag)
{
	FDialogTagHandle NewHandle = FDialogTagHandle(Tag);
	NewHandle.RenameTag(Tag->GetFullName());
	// Tag->Instances.Add(&NewHandle);
	return NewHandle;
}

void UDialogTagManager::AppendChildTags(FJsonDomBuilder::FObject* Parent, FDialogTag& Tag)
{
	for (int32 ChildId : Tag.Children)
	{
		if (!Tags.IsValidIndex(ChildId))
		{
			continue;
		}
		FDialogTag& ChildTag = Tags[ChildId];
		FJsonDomBuilder::FObject ChildObject;
		if (!ChildTag.Children.IsEmpty())
		{
			AppendChildTags(&ChildObject, ChildTag);
		}
		FJsonValueNull Empty = FJsonValueNull();
		Parent->Set(ChildTag.Tag.ToString(), ChildObject);
	}
}


void UDialogTagManager::GenerateTagsFromChildren(TSharedPtr<FJsonObject> Parent, int32 ParentIndex)
{
	for (TTuple<FString, JsonValPtr> JsonPair : Parent->Values)
	{
		FName TagName = FName(JsonPair.Get<FString>());
		int32 NewIndex = CreateTag(TagName, ParentIndex);
		JsonValPtr Json = JsonPair.Get<JsonValPtr>();
		if (Json->Type == EJson::Object)
		{
			GenerateTagsFromChildren(Json->AsObject(), NewIndex);
		}
	}
}

void UDialogTagManager::LoadFile()
{
	FArchive* Ar = IFileManager::Get().CreateFileReader(*GetFileName());
	if (!Ar)
	{
		// Failed to create the file reader...
		UE_LOG(LogTemp, Warning, TEXT("Failed to load DialogTag file."));
	}
	else
	{
		TSharedRef<TJsonReader<CharType>> JsonReader = TJsonReader<CharType>::Create(Ar);
		JsonObjPtr JsonObject = MakeShareable(new FJsonObject);
		if (!FJsonSerializer::Deserialize(JsonReader, JsonObject, FJsonSerializer::EFlags::None))
		{
			UE_LOG(LogTemp, Error, TEXT("Json deserializer error state: %s"), *JsonReader->GetErrorMessage());
		}
		delete Ar;


		for (TTuple<FString, JsonValPtr> JsonPair : JsonObject->Values)
		{
			FName TagName = FName(JsonPair.Get<FString>());
			int32 Index = CreateTag(TagName);
			JsonValPtr Json = JsonPair.Get<JsonValPtr>();
			if (Json->Type == EJson::Object)
			{
				JsonObjPtr Subobject = Json->AsObject();
				GenerateTagsFromChildren(Subobject, Index);
			}
		}
	}
	bIsLoaded = true;
}


void UDialogTagManager::SaveFile()
{
	if (!bIsLoaded)
	{
		return;
	}
	FJsonDomBuilder::FObject Root;
	AppendChildTags(&Root, RootTag);

	if (FArchive* Ar = IFileManager::Get().CreateFileWriter(*GetFileName()))
	{
		TSharedRef<TJsonWriter<CharType>> JsonWriter = TJsonWriter<CharType>::Create(Ar);
		FJsonSerializer::Serialize(Root.AsJsonObject(), JsonWriter);
		delete Ar;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create file writer for Json serialization."));
	}
}


const FString UDialogTagManager::GetFileName()
{
	const FString FileLocation = FPaths::GeneratedConfigDir();
	FString FileName = TAGFILENAME;
	FPaths::MakePlatformFilename(FileName);
	return FPaths::Combine(FileLocation, FileName);
}
