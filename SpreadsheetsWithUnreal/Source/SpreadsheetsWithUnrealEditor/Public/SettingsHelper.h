// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealEd.h"

/**
 * 
 */
class SPREADSHEETSWITHUNREALEDITOR_API SettingsHelper
{
public:
	SettingsHelper();
	~SettingsHelper();

    static bool ExistsInExcludeDirectories(FString Path);
    static void AddToProjectExcludeDirectories(FString Path);
};
