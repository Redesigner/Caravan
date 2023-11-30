#include "CaravanAbility/System/GameplayDebuggerCategory_CaravanAbility.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "AbilitySystemGlobals.h"

#include "CaravanAbility/GameplayAbilities/Components/CharacterAbilitySystemComponent.h"

FGameplayDebuggerCategory_CaravanAbility::FGameplayDebuggerCategory_CaravanAbility()
{
	SetDataPackReplication<FRepData>(&DataPack);
	bShowCategoryName = true;
	bShowOnlyWithDebugActor = false;
}

void FGameplayDebuggerCategory_CaravanAbility::CollectData(APlayerController *OwnerPC, AActor *DebugActor)
{
	if (!DebugActor)
	{
		DebugActor = OwnerPC->GetPawn();
	}

	const UCharacterAbilitySystemComponent* AbilityComponent = Cast<UCharacterAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DebugActor));

	if (!AbilityComponent)
	{
		return;
	}

	DataPack.UserQueuedAbilities = AbilityComponent->GetQueuedAbilities();
}


void FGameplayDebuggerCategory_CaravanAbility::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Printf(TEXT("Ability Queue:"));

	for (TPair<float, FGameplayTag> QueuedAbility : DataPack.UserQueuedAbilities)
	{
		CanvasContext.Printf(TEXT("\t{yellow}%s {grey}: %f "), *QueuedAbility.Value.ToString(), QueuedAbility.Key);
	}
}


TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_CaravanAbility::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_CaravanAbility());
}

void FGameplayDebuggerCategory_CaravanAbility::FRepData::Serialize(FArchive& Ar)
{
	int32 AbilitiesNum;
	if (Ar.IsSaving())
	{
		AbilitiesNum = static_cast<int32>(UserQueuedAbilities.Num());
	}
	Ar << AbilitiesNum;
	if (Ar.IsLoading())
	{
		UserQueuedAbilities.SetNumZeroed(AbilitiesNum, false);
	}
	for (TPair<float, FGameplayTag> QueuedAbility : UserQueuedAbilities)
	{
		Ar << QueuedAbility.Key;
		Ar << QueuedAbility.Value;
	}
}

#endif