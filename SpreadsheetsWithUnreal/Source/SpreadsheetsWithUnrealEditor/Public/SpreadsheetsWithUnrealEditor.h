// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

#define SPREADSHEETS_WITH_UNREAL_EDITOR_MODULE_NAME "SpreadsheetsWithUnrealEditor"

class FSpreadsheetsWithUnrealEditorModule : public IModuleInterface {
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
