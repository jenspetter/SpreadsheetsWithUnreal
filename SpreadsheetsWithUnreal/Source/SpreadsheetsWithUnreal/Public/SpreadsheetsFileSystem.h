// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Engine/DataTable.h"
#include "Settings/CredentialSettings.h"

#include "SpreadsheetsFileSystem.generated.h"
/**
 *
 */

class UDataTable;

UCLASS()
class SPREADSHEETSWITHUNREAL_API USpreadsheetsFileSystem : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Import CSV To Data Table", Keywords = "Data table import"), Category = "Spreadsheets With Unreal | Import")
	static void ImportCSVToDataTable(const FString& PathToCSVFile, UDataTable* DataTable);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Spreadsheet Credentials", Keywords = "Spreadsheets settings"), Category = "Spreadsheets With Unreal | Settings")
	static FSpreadsheetCrendentials GetSpreadsheetCredentials(const FString& CredentialName);
};
