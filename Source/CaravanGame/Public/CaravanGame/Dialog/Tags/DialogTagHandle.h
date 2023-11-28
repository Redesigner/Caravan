// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

#include "DialogTagHandle.generated.h"

/**
 * 
 */
struct FDialogTag;

USTRUCT(BlueprintType, Atomic)
struct CARAVANGAME_API FDialogTagHandle
{
	GENERATED_BODY()

	FDialogTagHandle();
	FDialogTagHandle(FDialogTag* Tag);

	void UpdateInternalTag();

public:
	bool Serialize(FArchive& Ar);

	FString GetFullName() const;

	void RenameTag(const FString& NewName);

	const FDialogTag* GetTag();

	FString TagName;

private:

	// A pointer to the actual tag object, currently stored inside of our DialogSubsystem
	// We will find this tag on construction, and it shouldn't be modified
	FDialogTag* OriginalTag = nullptr;
};

template<>
struct TStructOpsTypeTraits<FDialogTagHandle> : public TStructOpsTypeTraitsBase2<FDialogTagHandle>
{
	enum
	{
		WithSerializer = true,
	};
};



FORCEINLINE FArchive& operator <<(FArchive& Ar, FDialogTagHandle& TagHandle);