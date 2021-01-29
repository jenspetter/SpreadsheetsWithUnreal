// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Http.h"
#include "HttpModule.h"
#include "Interfaces/IPluginManager.h"

#include "Requests/Request.h"
#include "Responses/Response.h"
#include "Settings/CredentialSettings.h"
#include "SpreadsheetsHttp.h"
#include "SpreadsheetsFileSystem.h"

#include "SpreadsheetsWithUnrealBPLibrary.generated.h"

/*
 *	Function library class.
 *	Each function in it is expected to be static and represents blueprint
 *node that can be called in any blueprint.
 *
 *	When declaring function you can define metadata for the node. Key
 *function specifiers will be BlueprintPure and BlueprintCallable. BlueprintPure
 *- means the function does not affect the owning object in any way and thus
 *creates a node without Exec pins. BlueprintCallable - makes a function which
 *can be executed in Blueprints - Thus it has Exec pins. DisplayName - full name
 *of the node, shown when you mouse over the node and in the blueprint drop down
 *menu. Its lets you name the node using characters not allowed in C++ function
 *names. CompactNodeTitle - the word(s) that appear on the node. Keywords -
 *the list of keywords that helps you to find node when you search for it using
 *Blueprint drop-down menu. Good example is "Print String" node which you can
 *find also by using keyword "log". Category -	the category your node will be
 *under in the Blueprint drop-down menu.
 *
 *	For more info on custom blueprint nodes visit documentation:
 *	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
 */

DECLARE_DYNAMIC_DELEGATE_TwoParams(FReadCellRequestFinishedDelegate, FReadCellResponse, ReadCellRequest, bool, Successful);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FReadRangeRequestFinishedDelegate, FReadRangeResponse, ReadRangeRequest, bool, Successful);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRequestFinishedDelegate, bool, Successful);

UCLASS()
class SPREADSHEETSWITHUNREAL_API USpreadsheetReadWrite : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (Keywords = "Spreadsheets Read"), Category = "Spreadsheets With Unreal | Read")
    static void ReadCell(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FReadCellRequest& CellRequest, const FReadCellRequestFinishedDelegate& OnReadCellRequestFinished);
    UFUNCTION(BlueprintCallable, meta = (Keywords = "Spreadsheets Read"), Category = "Spreadsheets With Unreal | Read")
    static void ReadRange(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FReadRangeRequest& RangeRequest, const FReadRangeRequestFinishedDelegate& OnReadRangeRequestFinished);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Spreadsheets Write"), Category = "Spreadsheets With Unreal | Write")
    static void WriteToCell(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FWriteToCellRequest& WriteToCellRequest, const FRequestFinishedDelegate& OnWriteToCellRequestFinished);
    UFUNCTION(BlueprintCallable, meta = (Keywords = "Spreadsheets Write"), Category = "Spreadsheets With Unreal | Write")
    static void WriteToRange(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FWriteToRangeRequest& WriteToRangeRequest, const FRequestFinishedDelegate& OnWriteToRangeRequestFinished);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Spreadsheets Append"), Category = "Spreadsheets With Unreal | Append")
    static void AppendToTable(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FAppendToTableRequest& AppendToTableRequest, const FRequestFinishedDelegate& OnAppendToTableRequestFinished);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Spreadsheets Clear"), Category = "Spreadsheets With Unreal | Clear")
    static void ClearCell(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FClearCellRequest& ClearCellRequest, const FRequestFinishedDelegate& OnClearCellRequestFinished);
    UFUNCTION(BlueprintCallable, meta = (Keywords = "Spreadsheets Clear"), Category = "Spreadsheets With Unreal | Clear")
    static void ClearRange(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FClearRangeRequest& ClearRangeRequest, const FRequestFinishedDelegate& OnClearRangeRequestFinished);

    UFUNCTION(BlueprintCallable, meta = (Keywords = "Spreadsheets Export"), Category = "Spreadsheets With Unreal | Export")
    static void ExportSpreadsheet(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FExportRequest& ExportRequest, const FRequestFinishedDelegate& OnExportRequestFinished);

private:
    static void OnRequestReadCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
    static void OnRequestReadRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

    static FString OnRequestWriteToCellStarted(const FBaseRequest& BaseRequest, const FWriteToCellRequest& WriteToCellRequest);
    static void OnRequestWriteToCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
    static FString OnRequestWriteToRangeStarted(const FBaseRequest& BaseRequest, const FWriteToRangeRequest& WriteToRangeRequest);
    static void OnRequestWriteToRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

    static FString OnRequestAppendToTableStarted(const FBaseRequest& BaseRequest, const FAppendToTableRequest& WriteToRangeRequest);
    static void OnRequestAppendToTableFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

    static void OnRequestClearCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
    static void OnRequestClearRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

    static void OnRequestExportFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

public:
    friend class FSpreadsheetsHttp;

private:
    static FReadCellRequestFinishedDelegate ReadCellRequestFinishedDelegate;
    static FReadRangeRequestFinishedDelegate ReadRangeRequestFinishedDelegate;

    static FRequestFinishedDelegate WriteToCellRequestFinishedDelegate;
    static FRequestFinishedDelegate WriteToRangeRequestFinishedDelegate;

    static FRequestFinishedDelegate AppendToTableRequestFinishedDelegate;

    static FExportRequest LocalExportRequest;
    static FRequestFinishedDelegate ExportRequestFinishedDelegate;

    static FRequestFinishedDelegate ClearCellRequestFinishedDelegate;
    static FRequestFinishedDelegate ClearRangeRequestFinishedDelegate;
};