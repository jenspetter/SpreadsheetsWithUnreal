// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/CredentialSettings.h"

#if WITH_EDITOR
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "LevelEditor.h"
#endif

#define LOCTEXT_NAMESPACE "UserSettingsMenu"
UCredentialSettings::UCredentialSettings(const FObjectInitializer &ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITOR
  if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
    // Register the settings
    ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(
        "Project", "Plugins", "Spreadsheets With Unreal",
        LOCTEXT("RuntimeGeneralSettingsName", "Spreadsheets With Unreal"),
        LOCTEXT("RuntimeGeneralSettingsDescription",
                "Project settings for the JSheets plugin"),
        GetMutableDefault<UCredentialSettings>());
  }
#endif
}
#undef LOCTEXT_NAMESPACE