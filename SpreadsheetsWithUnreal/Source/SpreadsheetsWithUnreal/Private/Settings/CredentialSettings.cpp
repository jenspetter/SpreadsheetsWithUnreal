// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/CredentialSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "LevelEditor.h"

UCredentialSettings::UCredentialSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#define LOCTEXT_NAMESPACE "UserSettingsMenu"
	if (ISettingsModule * SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "JSheets",
			LOCTEXT("RuntimeGeneralSettingsName", "JSheets"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Project settings for the JSheets plugin"),
			GetMutableDefault<UCredentialSettings>()
		);
	}
#undef LOCTEXT_NAMESPACE
}