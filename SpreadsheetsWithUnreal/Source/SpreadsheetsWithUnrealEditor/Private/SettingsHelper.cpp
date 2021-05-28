// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsHelper.h"
#include "Settings/ProjectPackagingSettings.h"
#include "Engine/EngineTypes.h"

SettingsHelper::SettingsHelper()
{
}

SettingsHelper::~SettingsHelper()
{
}

bool SettingsHelper::ExistsInExcludeDirectories(FString Path)
{
    UProjectPackagingSettings* ProjectPackagingSettings = GetMutableDefault<UProjectPackagingSettings>();
    FDirectoryPath path;
    path.Path = Path;

    bool found = false;
    
    for(int i = 0; i < ProjectPackagingSettings->DirectoriesToAlwaysStageAsUFS.Num(); ++i)
    {
        if(ProjectPackagingSettings->DirectoriesToAlwaysStageAsUFS[i].Path == Path)
        {
            found = true;
        }
    }


    return found;
}

void SettingsHelper::AddToProjectExcludeDirectories(FString Path)
{
    UProjectPackagingSettings* ProjectPackagingSettings = GetMutableDefault<UProjectPackagingSettings>();
    FDirectoryPath path;
    path.Path = Path;
    ProjectPackagingSettings->DirectoriesToAlwaysStageAsUFS.Add(path);
}
