// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Requests/Request.h"
#include "Responses/Response.h"
#include "Settings/CredentialSettings.h"
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

class UDataTable;

UENUM()
enum ESpreadsheetHttpRequestType
{
    GET,
    PUT,
    POST
};

UENUM()
enum ESpreadsheetReadWriteType
{
    ReadCell,
    ReadRange,
    Export,
    WriteToCell,
    WriteToRange,
    AppendToTable,
    ClearCell,
    ClearRange,
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FReadCellRequestFinished, FReadCellResponse, ReadCellRequest, bool, Successful);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FReadRangeRequestFinished, FReadRangeResponse, ReadRangeRequest, bool, Successful);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRequestFinished, bool, Successful);

class USpreadsheetHttp
{
public:
    static void SendRequest(const FSpreadsheetCrendentials Credentials, const FString Url, const TEnumAsByte<ESpreadsheetHttpRequestType> HttpRequestType, const TEnumAsByte<ESpreadsheetReadWriteType> SpreadsheetRequestType, const FString Body = FString(""));

    static FString SetupTokenRequest(const FSpreadsheetCrendentials Credentials);
};

UCLASS()
class USpreadsheetFileSystem : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Import CSV To Data Table", Keywords = "Data table import"), Category = "Spreadsheets With Unreal | Import")
    static void ImportCSVToDataTable(const FString PathToCSVFile, UDataTable *DataTable);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Spreadsheet Credentials", Keywords = "Spreadsheets settings"), Category = "Spreadsheets With Unreal | Settings")
    static FSpreadsheetCrendentials GetSpreadsheetCredentials(const FString CredentialName);
};

UCLASS()
class USpreadsheetReadWrite : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Cell", Keywords = "Spreadsheets Read"), Category = "Spreadsheets With Unreal | Read")
    static void ReadCell(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FReadCellRequest CellRequest, const FReadCellRequestFinished &OnReadCellRequestFinished);
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Range", Keywords = "Spreadsheets Read"), Category = "Spreadsheets With Unreal | Read")
    static void ReadRange(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FReadRangeRequest RangeRequest, const FReadRangeRequestFinished &OnReadRangeRequestFinished);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Write To Cell", Keywords = "Spreadsheets Write"), Category = "Spreadsheets With Unreal | Write")
    static void WriteToCell(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FWriteToCellRequest WriteToCellRequest, const FRequestFinished &OnWriteToCellRequestFinished);
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Write To Range", Keywords = "Spreadsheets Write"), Category = "Spreadsheets With Unreal | Write")
    static void WriteToRange(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FWriteToRangeRequest WriteToRangeRequest, const FRequestFinished &OnWriteToRangeRequestFinished);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Clear Cell", Keywords = "Spreadsheets Clear"), Category = "Spreadsheets With Unreal | Clear")
    static void ClearCell(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FClearCellRequest ClearCellRequest, const FRequestFinished &OnClearCellRequestFinished);
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Clear Range", Keywords = "Spreadsheets Clear"), Category = "Spreadsheets With Unreal | Clear")
    static void ClearRange(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FClearRangeRequest ClearRangeRequest, const FRequestFinished &OnClearRangeRequestFinished);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Spreadsheet", Keywords = "Spreadsheets Export"), Category = "Spreadsheets With Unreal | Export")
    static void Export(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FExportRequest ExportRequest, const FRequestFinished& OnExportRequestFinished);

private:
    static void OnRequestReadCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
    static void OnRequestReadRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

    static FString OnRequestWriteToCellStarted(const FBaseRequest& BaseRequest, const FWriteToCellRequest& WriteToCellRequest);
    static void OnRequestWriteToCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
    static FString OnRequestWriteToRangeStarted(const FBaseRequest& BaseRequest, const FWriteToRangeRequest& WriteToRangeRequest);
    static void OnRequestWriteToRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

    static void OnRequestClearCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
    static void OnRequestClearRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

    static void OnRequestExportFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

public:
    friend class USpreadsheetHttp;

private:
    static FReadCellRequestFinished ReadCellRequestFinished;
    static FReadRangeRequestFinished ReadRangeRequestFinished;

    static FRequestFinished WriteToCellRequestFinished;
    static FRequestFinished WriteToRangeRequestFinished;

    static FExportRequest LocalExportRequest;
    static FRequestFinished ExportRequestFinished;

    static FRequestFinished ClearCellRequestFinished;
    static FRequestFinished ClearRangeRequestFinished;
};