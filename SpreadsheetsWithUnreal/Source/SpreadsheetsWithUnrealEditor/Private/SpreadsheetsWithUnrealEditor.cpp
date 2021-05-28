// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpreadsheetsWithUnrealEditor.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

#define LOCTEXT_NAMESPACE "SpreadsheetsWithUnrealEditor"

void FSpreadsheetsWithUnrealEditorModule::StartupModule()
{
    //UE_LOG(TutorialEditor, Warning, TEXT("TutorialEditor: Log Started"));
}

void FSpreadsheetsWithUnrealEditorModule::ShutdownModule()
{
    //UE_LOG(TutorialEditor, Warning, TEXT("TutorialEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSpreadsheetsWithUnrealEditorModule, SpreadsheetsWithUnrealEditor)
