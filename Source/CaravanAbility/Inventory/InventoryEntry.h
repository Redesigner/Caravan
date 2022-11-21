// Copyright 2022 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

#include "InventoryEntry.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CARAVANABILITY_API FInventoryEntry
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
