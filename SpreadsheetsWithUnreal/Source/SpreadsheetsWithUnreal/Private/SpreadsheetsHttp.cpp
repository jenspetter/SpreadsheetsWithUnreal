// Fill out your copyright notice in the Description page of Project Settings.


#include "SpreadsheetsHttp.h"

#include "Http.h"
#include "HttpModule.h"
#include "Interfaces/IPluginManager.h"

#include "SpreadsheetsWithUnrealBPLibrary.h"

void FSpreadsheetsHttp::SendRequest(const FSpreadsheetCrendentials Credentials, const FString Url, const TEnumAsByte<ERequestType> HttpRequestType, const TEnumAsByte<EReadWriteType> SpreadsheetRequestType, const FString Body)
{
	const FString tokenUrl = SetupTokenRequest(Credentials);

	TSharedRef<IHttpRequest> tokenRequest = FHttpModule::Get().CreateRequest();

	FHttpRequestCompleteDelegate& tokenDelegate = tokenRequest->OnProcessRequestComplete();

	tokenDelegate.BindLambda([&, Url, HttpRequestType, SpreadsheetRequestType, Body](FHttpRequestPtr request, FHttpResponsePtr response, bool success) {
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*response->GetContentAsString());

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			const FString accessToken = JsonObject->GetStringField("access_token");
			const FString extendedUrl = Url + accessToken;

			TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

			FHttpRequestCompleteDelegate& del = Request->OnProcessRequestComplete();

			switch (SpreadsheetRequestType)
			{
			case EReadWriteType::ReadCell:
				del.BindStatic(&USpreadsheetReadWrite::OnRequestReadCellFinished);
				break;
			case EReadWriteType::ReadRange:
				del.BindStatic(&USpreadsheetReadWrite::OnRequestReadRangeFinished);
				break;
			case EReadWriteType::WriteToCell:
				del.BindStatic(&USpreadsheetReadWrite::OnRequestWriteToCellFinished);
				break;
			case EReadWriteType::WriteToRange:
				del.BindStatic(&USpreadsheetReadWrite::OnRequestWriteToRangeFinished);
				break;
			case EReadWriteType::AppendToTable:
				//Setting content type header purely for appending since that is treated as as a 
				//streamed http request
				Request->SetHeader("Content-Type", "application/json");
				del.BindStatic(&USpreadsheetReadWrite::OnRequestAppendToTableFinished);
				break;
			case EReadWriteType::ClearCell:
				del.BindStatic(&USpreadsheetReadWrite::OnRequestClearCellFinished);
				break;
			case EReadWriteType::ClearRange:
				del.BindStatic(&USpreadsheetReadWrite::OnRequestClearRangeFinished);
				break;
			case EReadWriteType::Export:
				del.BindStatic(&USpreadsheetReadWrite::OnRequestExportFinished);
				break;
			}

			Request->SetURL(extendedUrl);

			switch (HttpRequestType)
			{
			case ERequestType::Get:
				Request->SetVerb("GET");
				break;
			case ERequestType::Put:
				Request->SetVerb("PUT");
				Request->SetContentAsString(Body);
				break;
			case ERequestType::Post:
				Request->SetVerb("POST");
				Request->SetContentAsString(Body);
				break;
			}

			Request->ProcessRequest();
		}
	});

	tokenRequest->SetURL(tokenUrl);
	tokenRequest->SetVerb("POST");
	tokenRequest->ProcessRequest();
}

FString FSpreadsheetsHttp::SetupTokenRequest(const FSpreadsheetCrendentials Credentials) {
	///Get file path and load into string
	const FString filePath = FPaths::ProjectContentDir() + FString("/") + Credentials.PathToOAuthCredentialsFile;
	FString fileData = "";
	FFileHelper::LoadFileToString(fileData, *filePath);

	FString clientId;
	FString clientSecret;
	const FString refreshToken = Credentials.RefreshToken;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(fileData);

	// Deserialize content of loaded file
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) {
		const TSharedPtr<FJsonObject> PersonObject = JsonObject->GetObjectField("web");

		// Assigning properties
		clientId = PersonObject->GetStringField("client_id");
		clientSecret = PersonObject->GetStringField("client_secret");
	}

	// Construct Url
	const FString url =
		"https://accounts.google.com/o/oauth2/token?client_id=" + clientId +
		FString("&client_secret=") + clientSecret + FString("&refresh_token=") +
		refreshToken + FString("&grant_type=refresh_token");

	return FString(url);
}
