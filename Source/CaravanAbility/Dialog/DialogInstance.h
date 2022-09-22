#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "GameplayTagContainer.h"
#include "DialogInstance.generated.h"

USTRUCT(BlueprintType)
struct CARAVANABILITY_API FDialogInstance : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Text = TEXT("None");

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanAdvance = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = bCanAdvance))
	FName NextDialog = TEXT("None");

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanRespond = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = bCanAdvance))
	TArray<FString> Responses;
};

