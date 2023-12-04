// Copyright (c) 2023 Stephen Melnick

#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"
#include "GameplayTagContainer.h" 

class FGameplayDebuggerCategory_CaravanAbility : public FGameplayDebuggerCategory
{
public:
	FGameplayDebuggerCategory_CaravanAbility();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;

	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;


	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

protected:
	struct FRepData
	{
		FString AbilityComponentOwnerDisplayName;

		TArray<TPair<float, FGameplayTag>> UserQueuedAbilities;

		void Serialize(FArchive& Ar);
	};

	FRepData DataPack;
};
#endif