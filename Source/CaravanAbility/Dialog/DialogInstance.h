#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "GameplayTagContainer.h"
#include "DialogInstance.generated.h"

USTRUCT(BlueprintType)
struct CARAVANABILITY_API FDialogInstance : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// FGameplayTag GameplayTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Text;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanAdvance;

	// UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = bCanAdvance) )
	// FGameplayTag NextDialogTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = bCanAdvance))
	FName NextDialog;
};

