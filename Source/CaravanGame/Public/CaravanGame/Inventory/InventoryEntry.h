// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

#include "InventoryEntry.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CARAVANGAME_API FInventoryEntry
{
	GENERATED_BODY()

public:
	static FInventoryEntry Empty();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FName DisplayName;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 ItemId;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int ItemCount;
};
