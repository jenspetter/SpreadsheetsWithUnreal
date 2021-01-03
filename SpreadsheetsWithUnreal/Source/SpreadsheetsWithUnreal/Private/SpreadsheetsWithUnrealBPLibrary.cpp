// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpreadsheetsWithUnrealBPLibrary.h"
#include "Http.h"
#include "HttpModule.h"
#include "Interfaces/IPluginManager.h"
#include "SpreadsheetsWithUnreal.h"
#include "JsonObjectConverter.h"

FReadCellRequestFinishedDelegate USpreadsheetReadWrite::ReadCellRequestFinishedDelegate;
FReadRangeRequestFinishedDelegate USpreadsheetReadWrite::ReadRangeRequestFinishedDelegate;

FRequestFinishedDelegate USpreadsheetReadWrite::WriteToCellRequestFinishedDelegate;
FRequestFinishedDelegate USpreadsheetReadWrite::WriteToRangeRequestFinishedDelegate;

FExportRequest USpreadsheetReadWrite::LocalExportRequest;
FRequestFinishedDelegate USpreadsheetReadWrite::ExportRequestFinishedDelegate;

FRequestFinishedDelegate USpreadsheetReadWrite::ClearCellRequestFinishedDelegate;
FRequestFinishedDelegate USpreadsheetReadWrite::ClearRangeRequestFinishedDelegate;

#pragma region Http
void USpreadsheetHttp::SendRequest(const FSpreadsheetCrendentials Credentials, const FString Url, const TEnumAsByte<ESpreadsheetHttpRequestType> HttpRequestType, const TEnumAsByte<ESpreadsheetReadWriteType> SpreadsheetRequestType, const FString Body)
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
                case ESpreadsheetReadWriteType::ReadCell:
                    del.BindStatic(&USpreadsheetReadWrite::OnRequestReadCellFinished);
                    break;
                case ESpreadsheetReadWriteType::ReadRange:
                    del.BindStatic(&USpreadsheetReadWrite::OnRequestReadRangeFinished);
                    break;
                case ESpreadsheetReadWriteType::WriteToCell:
                    del.BindStatic(&USpreadsheetReadWrite::OnRequestWriteToCellFinished);
                    break;
                case ESpreadsheetReadWriteType::WriteToRange:
                    del.BindStatic(&USpreadsheetReadWrite::OnRequestWriteToRangeFinished);
                    break;
                case ESpreadsheetReadWriteType::ClearCell:
                    del.BindStatic(&USpreadsheetReadWrite::OnRequestClearCellFinished);
                    break;
                case ESpreadsheetReadWriteType::ClearRange:
                    del.BindStatic(&USpreadsheetReadWrite::OnRequestClearRangeFinished);
                    break;
                case ESpreadsheetReadWriteType::Export:
                    del.BindStatic(&USpreadsheetReadWrite::OnRequestExportFinished);
                    break;
            }

            Request->SetURL(extendedUrl);

            switch (HttpRequestType)
            {
                case ESpreadsheetHttpRequestType::GET:
                    Request->SetVerb("GET");
                    break;
                case ESpreadsheetHttpRequestType::PUT:
                    Request->SetVerb("PUT");
                    Request->SetContentAsString(Body);
                    break;
                case ESpreadsheetHttpRequestType::POST:
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

FString USpreadsheetHttp::SetupTokenRequest(const FSpreadsheetCrendentials Credentials) {
    ///Get file path and load into string
    const FString filePath = FPaths::ProjectContentDir() + FString("/") + Credentials.PathToOAuthCredentialsFile;
    FString fileData = "";
    FFileHelper::LoadFileToString(fileData, *filePath);

    FString clientId;
    FString clientSecret;
    const FString refreshToken = Credentials.RefreshToken;

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(fileData);

    //Deserialize content of loaded file
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) {
        const TSharedPtr<FJsonObject> PersonObject = JsonObject->GetObjectField("web");

        //Assigning properties
        clientId = PersonObject->GetStringField("client_id");
        clientSecret = PersonObject->GetStringField("client_secret");
    }

    //Construct Url
    const FString url =
        "https://accounts.google.com/o/oauth2/token?client_id=" + clientId +
        FString("&client_secret=") + clientSecret + FString("&refresh_token=") +
        refreshToken + FString("&grant_type=refresh_token");

    return FString(url);
}
#pragma endregion Http

#pragma region FileSystem
USpreadsheetFileSystem::USpreadsheetFileSystem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void USpreadsheetFileSystem::ImportCSVToDataTable(const FString PathToCSVFile, UDataTable* DataTable)
{
    FString FileContent;

    FString path = FPaths::ProjectContentDir() + PathToCSVFile;
    FFileHelper::LoadFileToString(FileContent, *path);

    if (DataTable) {
        TArray<FString> problems = DataTable->CreateTableFromCSVString(FileContent);

        if (problems.Num() > 0) {
            for (int32 ProbIdx = 0; ProbIdx < problems.Num(); ProbIdx++) {
                // Erros detected
            }
            UE_LOG(LogTemp, Warning, TEXT("Data Table update failed"));
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Data Table updated successfully"));
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Data Table not found"));
    }
}

FSpreadsheetCrendentials USpreadsheetFileSystem::GetSpreadsheetCredentials(const FString CredentialName) {
    UCredentialSettings* m_EditorSettingsSingleton = nullptr;

    if (!m_EditorSettingsSingleton) {
        static const TCHAR* settingsContainerName = TEXT("SpreadsheetsWithUnreal");
        m_EditorSettingsSingleton = FindObject<UCredentialSettings>(GetTransientPackage(), settingsContainerName);

        if (!m_EditorSettingsSingleton) {
            m_EditorSettingsSingleton = NewObject<UCredentialSettings>(GetTransientPackage(), UCredentialSettings::StaticClass(), settingsContainerName);
            m_EditorSettingsSingleton->AddToRoot();
        }
    }

    for (auto it = m_EditorSettingsSingleton->CredentialOptions.begin();
        it != m_EditorSettingsSingleton->CredentialOptions.end(); ++it) {

        if (it.Key() == CredentialName) {
            return it.Value();
        }
    }

    return FSpreadsheetCrendentials();
}
#pragma endregion FileSystem

#pragma region ReadWrite
USpreadsheetReadWrite::USpreadsheetReadWrite(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

void USpreadsheetReadWrite::ReadCell(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FReadCellRequest CellRequest, const FReadCellRequestFinishedDelegate &OnReadCellRequestFinished)
{
    ReadCellRequestFinishedDelegate = OnReadCellRequestFinished;

    //Construct Url
    FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
                BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
                FString("!") + CellRequest.Cell + FString("?access_token=");

    USpreadsheetHttp::SendRequest(Credentials, url, ESpreadsheetHttpRequestType::GET, ESpreadsheetReadWriteType::ReadCell);
}

void USpreadsheetReadWrite::ReadRange(const FSpreadsheetCrendentials Credentials, const struct FBaseRequest BaseRequest, const struct FReadRangeRequest RangeRequest, const FReadRangeRequestFinishedDelegate &OnReadRangeRequestFinished)
{
    ReadRangeRequestFinishedDelegate = OnReadRangeRequestFinished;

    //Construct Url
    FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
                BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
                FString("!") + RangeRequest.Range + FString("?access_token=");

    USpreadsheetHttp::SendRequest(Credentials, url, ESpreadsheetHttpRequestType::GET, ESpreadsheetReadWriteType::ReadRange);
}

void USpreadsheetReadWrite::WriteToCell(const FSpreadsheetCrendentials Credentials, const FBaseRequest BaseRequest, const FWriteToCellRequest WriteToCellRequest, const FRequestFinishedDelegate& OnWriteToCellRequestFinished)
{
    WriteToCellRequestFinishedDelegate = OnWriteToCellRequestFinished;

    // Construct Url
    FString url = "https://sheets.googleapis.com/v4/spreadsheets/" + BaseRequest.SheetId + FString("/values/") +
        BaseRequest.Tab + FString("!") + WriteToCellRequest.CellToWriteTo + FString("?valueInputOption=RAW&access_token=");

    //Get parsed body
    FString body = OnRequestWriteToCellStarted(BaseRequest, WriteToCellRequest);

    USpreadsheetHttp::SendRequest(Credentials, url, ESpreadsheetHttpRequestType::PUT, ESpreadsheetReadWriteType::WriteToCell, body);
}

void USpreadsheetReadWrite::WriteToRange(const FSpreadsheetCrendentials Credentials, const FBaseRequest BaseRequest, const FWriteToRangeRequest WriteToRangeRequest, const FRequestFinishedDelegate& OnWriteToRangeRequestFinished)
{
    WriteToRangeRequestFinishedDelegate = OnWriteToRangeRequestFinished;

    // Construct Url
    FString url = "https://sheets.googleapis.com/v4/spreadsheets/" + BaseRequest.SheetId + FString("/values/") +
        BaseRequest.Tab + FString("!") + WriteToRangeRequest.RangeToWriteTo +
        FString("?valueInputOption=RAW&access_token=");

    // Get parsed body
    FString body = OnRequestWriteToRangeStarted(BaseRequest, WriteToRangeRequest);

    USpreadsheetHttp::SendRequest(Credentials, url, ESpreadsheetHttpRequestType::PUT, ESpreadsheetReadWriteType::WriteToRange, body);
}

void USpreadsheetReadWrite::ClearCell(const FSpreadsheetCrendentials Credentials, const FBaseRequest BaseRequest, const FClearCellRequest ClearCellRequest, const FRequestFinishedDelegate& OnClearCellRequestFinished)
{
    ClearCellRequestFinishedDelegate = OnClearCellRequestFinished;

    //Construct Url
    FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
        BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
        FString("!") + ClearCellRequest.Cell + FString(":clear?access_token=");

    USpreadsheetHttp::SendRequest(Credentials, url, ESpreadsheetHttpRequestType::POST, ESpreadsheetReadWriteType::ClearCell);
}

void USpreadsheetReadWrite::ClearRange(const FSpreadsheetCrendentials Credentials, const FBaseRequest BaseRequest, const FClearRangeRequest ClearRangeRequest, const FRequestFinishedDelegate& OnClearRangeRequestFinished)
{
    ClearRangeRequestFinishedDelegate = OnClearRangeRequestFinished;

    //Construct Url
    FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
        BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
        FString("!") + ClearRangeRequest.Range + FString(":clear?access_token=");

    USpreadsheetHttp::SendRequest(Credentials, url, ESpreadsheetHttpRequestType::POST, ESpreadsheetReadWriteType::ClearRange);
}

void USpreadsheetReadWrite::Export(const FSpreadsheetCrendentials Credentials, const FBaseRequest BaseRequest, const FExportRequest ExportRequest, const FRequestFinishedDelegate& OnExportRequestFinished) {
    ExportRequestFinishedDelegate = OnExportRequestFinished;
    LocalExportRequest = ExportRequest;

    //Construct Url
    FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
        BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
        FString("!") + ExportRequest.Range + FString("?access_token=");

    USpreadsheetHttp::SendRequest(Credentials, url, ESpreadsheetHttpRequestType::GET, ESpreadsheetReadWriteType::Export);
}

void USpreadsheetReadWrite::OnRequestReadCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    FReadCellResponse response;

    if (Success) {
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
        const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*Response->GetContentAsString());

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) {
            auto doubleArrayResponse = JsonObject->GetArrayField("values");
            auto elementOneResponse = doubleArrayResponse[0]->AsArray();
            response.Cell = elementOneResponse[0]->AsString();
        }
    }

    ReadCellRequestFinishedDelegate.Execute(response, Success);
}

void USpreadsheetReadWrite::OnRequestReadRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    FReadRangeResponse response;
    if (Success) {
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
        TSharedRef<TJsonReader<>> JsonReader =
        TJsonReaderFactory<>::Create(*Response->GetContentAsString());

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) {
            auto firstDimension = JsonObject->GetArrayField("values");

            for (auto dimensionElement: firstDimension) {
                auto dimensionElementAsArray = dimensionElement->AsArray();
                FSpreadsheetArray spreadsheetArray;

                for (auto arrayElement : dimensionElementAsArray) {
                    spreadsheetArray.Elements.Add(arrayElement->AsString());
                }

                response.Range.Add(spreadsheetArray);
            }
        }
    }

    ReadRangeRequestFinishedDelegate.Execute(response, Success);
}

FString USpreadsheetReadWrite::OnRequestWriteToCellStarted(const FBaseRequest& BaseRequest, const FWriteToCellRequest& WriteToCellRequest)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    const FString range = BaseRequest.Tab + FString("!") + WriteToCellRequest.CellToWriteTo;
    JsonObject->SetStringField("range", range);
    JsonObject->SetStringField("majorDimension", "ROWS");

    TArray<TSharedPtr<FJsonValue>> firstDimension;
    TArray<TSharedPtr<FJsonValue>> spreadsheetArray;
    spreadsheetArray.Add(MakeShareable(new FJsonValueString(WriteToCellRequest.NewCellContent)));
    firstDimension.Add(MakeShareable(new FJsonValueArray(spreadsheetArray)));

    JsonObject->SetArrayField("values", firstDimension);

    FString OutputString;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return FString(OutputString);
}

void USpreadsheetReadWrite::OnRequestWriteToCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    WriteToCellRequestFinishedDelegate.Execute(Success);
}

FString USpreadsheetReadWrite::OnRequestWriteToRangeStarted(const FBaseRequest& BaseRequest, const FWriteToRangeRequest& WriteToRangeRequest)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    const FString range = BaseRequest.Tab + FString("!") + WriteToRangeRequest.RangeToWriteTo;
    JsonObject->SetStringField("range", range);

    if(WriteToRangeRequest.Dimension == ESpreadsheetDimension::COLUMNS)
    {
        JsonObject->SetStringField("majorDimension", "COLUMNS");
    }
    else
    {
        JsonObject->SetStringField("majorDimension", "ROWS");
    }

    TArray<TSharedPtr<FJsonValue>> firstDimension;

    for (FSpreadsheetArray spreadsheetArray : WriteToRangeRequest.NewRangeContent)
    {
        TArray<TSharedPtr<FJsonValue>> spreadsheetJsonArray;
        
        for (FString element : spreadsheetArray.Elements)
        {
            spreadsheetJsonArray.Add(MakeShareable(new FJsonValueString(element)));
        }

        firstDimension.Add(MakeShareable(new FJsonValueArray(spreadsheetJsonArray)));
    }
    
    JsonObject->SetArrayField("values", firstDimension);

    FString OutputString;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    UE_LOG(LogTemp, Warning, TEXT("%S"), *OutputString);
    return FString(OutputString);
}

void USpreadsheetReadWrite::OnRequestWriteToRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    WriteToRangeRequestFinishedDelegate.Execute(Success);
}

void USpreadsheetReadWrite::OnRequestClearCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    ClearCellRequestFinishedDelegate.Execute(Success);
}

void USpreadsheetReadWrite::OnRequestClearRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    ClearRangeRequestFinishedDelegate.Execute(Success);
}

void USpreadsheetReadWrite::OnRequestExportFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    if (Success) {
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*Response->GetContentAsString());

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()) {
            auto doubleArrayResponse = JsonObject->GetArrayField("values");
            FString content = "";

            for (auto arr : doubleArrayResponse) {
                auto elResponse = arr->AsArray();
                for (int i = 0; i < elResponse.Num(); ++i) {
                    FString element = elResponse[i]->AsString();

                    if (i != 0) {
                        content += FString(",");
                    }

                    if (element.Contains(",")) {
                        content += FString("\"") + element += FString("\"");
                    }
                    else {
                        content += element;
                    }
                }
                content += FString("\n");
            }

            FString path = FPaths::ProjectContentDir() + LocalExportRequest.OutputDestination;

            FFileHelper::SaveStringToFile(content, *path);
        }
    }

    ExportRequestFinishedDelegate.Execute(Success);
}
#pragma endregion