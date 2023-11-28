// Copyright (c) 2023 Stephen Melnick

#pragma once

#include "Modules/ModuleInterface.h" 

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDialogEditor, Log, All);

class FCaravanDialogEditorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void* CustomRejectionTest = nullptr;
};
