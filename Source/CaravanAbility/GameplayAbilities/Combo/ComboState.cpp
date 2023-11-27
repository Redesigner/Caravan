// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboState.h"

#include "CaravanAbility/CaravanAbility.h"

FComboState FComboState::None()
{
    FComboState ComboState;
    ComboState.Name = TEXT("None");
    return ComboState;
}

bool FComboState::GetGameplayTagFromInput(int32 InputID, FGameplayTag& OutGameplayTag) const
{
    if (InputBranches.Contains(InputID))
    {
        OutGameplayTag = InputBranches.FindRef(InputID);
        UE_LOG(LogAbilityCombos, Display, TEXT("Player pressed input '%i', and queued next ability with tag '%s'"), InputID, *OutGameplayTag.ToString())
        return true;
    }
    // UE_LOG(LogAbilityCombos, Display, TEXT("Player pressed input '%i', but there was no ability associated with that input for combo state '%s'."), InputID, *Name.ToString())
    return false;
}

bool operator==(const FComboState& A, const FComboState& B)
{
    return A.Name == B.Name && &A.InputBranches == &B.InputBranches;
}

bool operator!=(const FComboState& A, const FComboState& B)
{
    return (A.Name != B.Name) ||
        (&A.InputBranches != &B.InputBranches);
}
