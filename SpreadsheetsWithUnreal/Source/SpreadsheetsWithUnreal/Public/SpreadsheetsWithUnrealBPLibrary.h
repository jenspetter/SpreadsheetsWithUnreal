// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Requests/Request.h"
#include "Responses/Response.h"
#include "Settings/CredentialSettings.h"
#include "SpreadsheetsWithUnrealBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

class UDataTable;

class UHttpRequest
{
public:
	static void Get(FSpreadsheetCrendentials a_Credentials, FString a_Url, UBaseResponse& a_ResponseObj);


	static FString GetTokenRequest(FSpreadsheetCrendentials a_Credentials);
};

UCLASS()
class USpreadsheetsWithUnrealBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Cell", Keywords = "JSheets reading"), Category = "JSheetsReading")
    static void ReadCell(FSpreadsheetCrendentials a_Credentials, struct FBaseRequest a_BaseRequest, struct FReadCellRequest a_CellRequest, const FReadCellRequestFinished& a_Callback);
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Range", Keywords = "JSheets reading"), Category = "JSheetsReading")
    static void ReadRange(FSpreadsheetCrendentials a_Credentials, struct FBaseRequest a_BaseRequest, struct FReadRangeRequest a_RangeRequest, const FReadRangeRequestFinished& a_Callback);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Spreadsheet", Keywords = "Spreadsheets export"), Category = "SpreadsheetsExport")
	static void Export(FSpreadsheetCrendentials a_Credentials, struct FBaseRequest a_BaseRequest, struct FExportRequest a_ExportRequest, UDataTable* a_DataTable, const FExportRequestFinished& a_Callback);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetSpreadsheetCredentials", Keywords = "Spreadsheets settings"), Category = "Settings")
    static FSpreadsheetCrendentials GetSpreadsheetCredentials(FString a_CredentialName);
};
