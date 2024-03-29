// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameplayTagContainer.h"
#include "ComboState.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CARAVANABILITY_API FComboState
{
	GENERATED_BODY()

public:
	/// An FComboState with the name "None" and no inputs
	static FComboState None();

	bool GetGameplayTagFromInput(int32 InputID, FGameplayTag& OutGameplayTag) const;

	/// Locally recognized name, for lookup
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Combo)
	FName Name;

	/// GameplayTags representing the abilities that can be played with this combo state
	/// Keys should correspond to the InputID enum
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Combo)
	TMap<int32, FGameplayTag> InputBranches;

	bool operator==(const FComboState& Other) const;

	bool operator!=(const FComboState& Other) const;
};
