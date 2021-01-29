// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Settings/CredentialSettings.h"

/**
 *
 */
class SPREADSHEETSWITHUNREAL_API FSpreadsheetsHttp
{
public:
	enum ERequestType
	{
		Get,
		Put,
		Post
	};

	enum EReadWriteType
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

	static void SendRequest(const FSpreadsheetCrendentials Credentials, const FString Url, const TEnumAsByte<ERequestType> HttpRequestType, const TEnumAsByte<EReadWriteType> SpreadsheetRequestType, const FString Body = FString(""));

	static FString SetupTokenRequest(const FSpreadsheetCrendentials Credentials);
};