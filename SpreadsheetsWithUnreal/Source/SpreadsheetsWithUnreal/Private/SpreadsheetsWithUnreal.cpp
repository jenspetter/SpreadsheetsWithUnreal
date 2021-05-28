// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpreadsheetsWithUnreal.h"
#include "Settings/CredentialSettings.h"

#if WITH_EDITOR
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "LevelEditor.h"
#endif

#define LOCTEXT_NAMESPACE "FSpreadsheetsWithUnrealModule"

void FSpreadsheetsWithUnrealModule::StartupModule() {
    RegisterModule();

    FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomClassLayout(UCredentialSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FSpreadsheetsSettingsDetails::MakeInstance));
}

void FSpreadsheetsWithUnrealModule::ShutdownModule() {
    if (UObjectInitialized())
    {
        UnregisterModule();

        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout(UCredentialSettings::StaticClass()->GetFName());
    }
}

void FSpreadsheetsWithUnrealModule::RegisterModule()
{
    if (ISettingsModule * SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
        // Register the settings
        ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(
            "Project", "Plugins", "Spreadsheets With Unreal",
            LOCTEXT("RuntimeGeneralSettingsName", "Spreadsheets With Unreal"),
            LOCTEXT("RuntimeGeneralSettingsDescription",
                "Project settings for the JSheets plugin"),
            GetMutableDefault<UCredentialSettings>());
    }
}

void FSpreadsheetsWithUnrealModule::UnregisterModule()
{
    if (ISettingsModule * SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Project", "Project", "Spreadsheets With Unreal");
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSpreadsheetsWithUnrealModule, SpreadsheetsWithUnreal)