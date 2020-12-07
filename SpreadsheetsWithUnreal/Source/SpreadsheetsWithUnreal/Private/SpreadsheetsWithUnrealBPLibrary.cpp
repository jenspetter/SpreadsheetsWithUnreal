// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpreadsheetsWithUnrealBPLibrary.h"
#include "HttpModule.h"
#include "Http.h"
#include "Interfaces/IPluginManager.h"
#include "SpreadsheetsWithUnreal.h"

USpreadsheetsWithUnrealBPLibrary::USpreadsheetsWithUnrealBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

//URequest::URequest(const FObjectInitializer& ObjectInitializer)
//{
//
//}
//
//void URequest::OnRequestComplete(FHttpRequestPtr request, FHttpResponsePtr response, bool success)
//{
//
//}

//void UJSheetsBPLibrary::ReadCell(FString sheetToken, FString sheetID, FString sheetRange, const FRequestFinished& Callback)
//{
//	UJSheetRequest* sheetRequest = NewObject<UJSheetRequest>(UJSheetRequest::StaticClass());
//	sheetRequest->m_Del = Callback;
//
//	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
//
//	FHttpRequestCompleteDelegate& del = Request->OnProcessRequestComplete();
//	del.BindUObject(sheetRequest, &UJSheetRequest::OnRequestFinished);
//	Request->SetURL("https://sheets.googleapis.com/v4/spreadsheets/" + sheetID + FString("/values/") + sheetRange + FString("?access_token=") + sheetToken);
//	Request->SetVerb("GET");
//	Request->ProcessRequest();
//}

void UHttpRequest::Get(FSpreadsheetCrendentials a_Credentials, FString a_Url, UBaseResponse& a_ResponseObj)
{
	FString url = GetTokenRequest(a_Credentials);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	FHttpRequestCompleteDelegate& del = Request->OnProcessRequestComplete();

	del.BindLambda([&, a_Url](FHttpRequestPtr request, FHttpResponsePtr response, bool success)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*response->GetContentAsString());

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			FString token = JsonObject->GetStringField("access_token");
			FString newUrl = a_Url + token;

			TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

			FHttpRequestCompleteDelegate& del = Request->OnProcessRequestComplete();

			del.BindUObject(&a_ResponseObj, &UBaseResponse::OnRequestFinished);
			Request->SetURL(newUrl);
			Request->SetVerb("GET");
			Request->ProcessRequest();
		}
	});

	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->ProcessRequest();
}

FString UHttpRequest::GetTokenRequest(FSpreadsheetCrendentials a_Credentials)
{
	FString contentData = IPluginManager::Get().FindPlugin(TEXT("JSheets"))->GetContentDir() + FString("/") + a_Credentials.m_PathToOAuthCredentialsFile;
	FString fileData = "";
	FFileHelper::LoadFileToString(fileData, *contentData);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(fileData);

	FString clientId;
	FString clientSecret;
	FString refreshToken = a_Credentials.m_RefreshToken;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *a_Credentials.m_PathToOAuthCredentialsFile);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *a_Credentials.m_RefreshToken);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		TSharedPtr<FJsonObject> PersonObject = JsonObject->GetObjectField("web");
		//Getting various properties
		clientId = PersonObject->GetStringField("client_id");
		clientSecret = PersonObject->GetStringField("client_secret");
	}

	FString url = "https://accounts.google.com/o/oauth2/token?client_id=" + clientId +
		FString("&client_secret=") + clientSecret +
		FString("&refresh_token=") + refreshToken +
		FString("&grant_type=refresh_token");

	return FString(url);
}

void USpreadsheetsWithUnrealBPLibrary::ReadCell(FSpreadsheetCrendentials a_Credentials, UReadCellRequest* a_Request,
	const FReadCellRequestFinished& a_Callback)
{
	UReadCellResponse* response = NewObject<UReadCellResponse>(UReadCellResponse::StaticClass());
	response->m_RequestFinishedDelegate = a_Callback;
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" + a_Request->m_SheetId + FString("/values/") +
		a_Request->m_Tab + FString("!") + a_Request->m_Cell + FString("?access_token=");

	UHttpRequest::Get(a_Credentials, url, *response);
}

void USpreadsheetsWithUnrealBPLibrary::ReadRange(FSpreadsheetCrendentials a_Credentials, UReadRangeRequest* a_Request,
	const FReadRangeRequestFinished& a_Callback)
{
	UReadRangeResponse* response = NewObject<UReadRangeResponse>(UReadRangeResponse::StaticClass());
	response->m_RequestFinishedDelegate = a_Callback;
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" + a_Request->m_SheetId + FString("/values/") +
		a_Request->m_Tab + FString("!") + a_Request->m_Range + FString("?access_token=");

	UHttpRequest::Get(a_Credentials, url, *response);
}