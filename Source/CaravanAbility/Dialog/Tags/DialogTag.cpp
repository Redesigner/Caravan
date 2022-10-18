// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogTag.h"

#include "CaravanAbility\Dialog\Tags\DialogTagHandle.h"
#include "CaravanAbility\Dialog\Tags\DialogTagManager.h"
#include "CaravanAbility\Dialog\DialogResponseInterface.h"

FDialogTag::FDialogTag()
{

}

FDialogTag::FDialogTag(FName& Tag)
	:Tag(Tag)
{
}

FDialogTag::FDialogTag(FName& Tag, int32 Parent)
	:Tag(Tag), Parent(Parent)
{

}

bool FDialogTag::operator==(const FDialogTag& a) const
{
	return Tag == a.Tag && Parent == a.Parent && Children == a.Children;
};

bool FDialogTag::HasChildren() const
{
	return !Children.IsEmpty();
}

void FDialogTag::AddChild(FName NewTagName)
{
	if (Owner == nullptr)
	{
		return;
	}
	for (int32 ChildIndex : Children)
	{
		if (Owner->Tags[ChildIndex].Tag == NewTagName)
		{
			// Tag already exists
			return;
		}
	}
	Owner->CreateTag(NewTagName, InternalIndex);
}

FName FDialogTag::GetName() const
{
	return Tag;
}

void FDialogTag::SetName(FName& Name)
{
	Tag = Name;
}

FString FDialogTag::GetFullName() const
{
	if (Owner == nullptr)
	{
		return FString();
	}
	const FDialogTag* CurrentTag = this;
	FString Name = Tag.ToString();
	while (CurrentTag->Parent != -1)
	{
		if (!Owner->Tags.IsValidIndex(CurrentTag->Parent))
		{
			return TEXT("Error");
		}
		CurrentTag = &Owner->Tags[CurrentTag->Parent];
		Name.InsertAt(0, '.');
		Name.InsertAt(0, CurrentTag->Tag.ToString());
	}
	return Name;
}

void FDialogTag::RenameInstances(const FString& NewFullName)
{
}

FArchive& operator<<(FArchive& Ar, FDialogTag& Tag)
{
	Ar << Tag.Tag;
	return Ar;
}
