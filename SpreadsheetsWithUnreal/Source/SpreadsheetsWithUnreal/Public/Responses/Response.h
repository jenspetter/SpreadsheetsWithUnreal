// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Http.h"
#include "Requests/Request.h"
#include "Engine/DataTable.h"
#include "Response.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_TwoParams(FTokenRequestFinished, class UTokenResponse*, TokenRequest, bool, Successful);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FReadCellRequestFinished, class UReadCellResponse*, ReadCellRequest, bool, Successful);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FReadRangeRequestFinished, class UReadRangeResponse*, ReadRangeRequest, bool, Successful);
DECLARE_DYNAMIC_DELEGATE_OneParam(FExportRequestFinished, bool, Successful);

UCLASS(BlueprintType, Blueprintable)
class SPREADSHEETSWITHUNREAL_API UBaseResponse : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success);

protected:
	bool m_RequestSuccessful;
};

UCLASS(BlueprintType, Blueprintable)
class SPREADSHEETSWITHUNREAL_API UTokenResponse : public UBaseResponse
{
	GENERATED_BODY()

public:
	void OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success);

public:
	FTokenRequestFinished m_RequestFinishedDelegate;
};

UCLASS(BlueprintType, Blueprintable)
class SPREADSHEETSWITHUNREAL_API UReadCellResponse : public UBaseResponse
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Cell", Keywords = "JSheets Response Reading"), Category = "JSheetsResponseReading")
		FString GetCell();

	void OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success);

public:
	FReadCellRequestFinished m_RequestFinishedDelegate;

private:
	FString m_Cell;
};

UCLASS(BlueprintType, Blueprintable)
class SPREADSHEETSWITHUNREAL_API UReadRangeResponse : public UBaseResponse
{
	GENERATED_BODY()

public:
	void OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success);

public:
	FReadRangeRequestFinished m_RequestFinishedDelegate;

private:
	TArray<FString> m_Range;
};

UCLASS(BlueprintType, Blueprintable)
class SPREADSHEETSWITHUNREAL_API UExportResponse : public UBaseResponse
{
	GENERATED_BODY()

public:
	void SetExportData(UDataTable* a_DataTable, TEnumAsByte<EExportFormat> a_ExportFormat, FString a_OutputDestination);
    void OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success);

public:
	FExportRequestFinished m_RequestFinishedDelegate;

private:
	UDataTable* m_DataTable;
	TEnumAsByte<EExportFormat> m_ExportFormat;
	FString m_OutputDestination;
};