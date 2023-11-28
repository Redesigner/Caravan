// Copyright (c) 2023 Stephen Melnick


#include "CaravanAbility/GameplayAbilities/Combo/ComboState.h"

#include "CaravanAbility.h"

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

bool FComboState::operator==(const FComboState& Other) const
{
    // Compare map by reference
    return Name == Other.Name && &InputBranches == &Other.InputBranches;
}

bool FComboState::operator!=(const FComboState& Other) const
{
    return (Name != Other.Name) ||
        (&InputBranches != &Other.InputBranches);
}
