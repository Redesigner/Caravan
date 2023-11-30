// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCaravanAbility, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogAbilityQueue, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogAbilityCombos, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(LogTargetingReticleSystem, Log, All);

class CARAVANABILITY_API FCaravanAbilityModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;
};