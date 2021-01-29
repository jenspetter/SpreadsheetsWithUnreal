// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpreadsheetsWithUnrealBPLibrary.h"
#include "SpreadsheetsWithUnreal.h"
#include "JsonObjectConverter.h"

FReadCellRequestFinishedDelegate USpreadsheetReadWrite::ReadCellRequestFinishedDelegate;
FReadRangeRequestFinishedDelegate USpreadsheetReadWrite::ReadRangeRequestFinishedDelegate;

FRequestFinishedDelegate USpreadsheetReadWrite::WriteToCellRequestFinishedDelegate;
FRequestFinishedDelegate USpreadsheetReadWrite::WriteToRangeRequestFinishedDelegate;

FRequestFinishedDelegate USpreadsheetReadWrite::AppendToTableRequestFinishedDelegate;

FExportRequest USpreadsheetReadWrite::LocalExportRequest;
FRequestFinishedDelegate USpreadsheetReadWrite::ExportRequestFinishedDelegate;

FRequestFinishedDelegate USpreadsheetReadWrite::ClearCellRequestFinishedDelegate;
FRequestFinishedDelegate USpreadsheetReadWrite::ClearRangeRequestFinishedDelegate;

USpreadsheetReadWrite::USpreadsheetReadWrite(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void USpreadsheetReadWrite::ReadCell(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FReadCellRequest& CellRequest, const FReadCellRequestFinishedDelegate& OnReadCellRequestFinished)
{
	ReadCellRequestFinishedDelegate = OnReadCellRequestFinished;

	// Construct Url
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
		BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
		FString("!") + CellRequest.CellLocation + FString("?access_token=");

	FSpreadsheetsHttp::SendRequest(Credentials, url, FSpreadsheetsHttp::ERequestType::Get, FSpreadsheetsHttp::EReadWriteType::ReadCell);
}

void USpreadsheetReadWrite::ReadRange(const FSpreadsheetCrendentials& Credentials, const struct FBaseRequest& BaseRequest, const struct FReadRangeRequest& RangeRequest, const FReadRangeRequestFinishedDelegate& OnReadRangeRequestFinished)
{
	ReadRangeRequestFinishedDelegate = OnReadRangeRequestFinished;

	// Construct Url
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
		BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
		FString("!") + RangeRequest.RangeLocationStart + FString(":") + RangeRequest.RangeLocationEnd + FString("?access_token=");

	FSpreadsheetsHttp::SendRequest(Credentials, url, FSpreadsheetsHttp::ERequestType::Get, FSpreadsheetsHttp::EReadWriteType::ReadRange);
}

void USpreadsheetReadWrite::WriteToCell(const FSpreadsheetCrendentials& Credentials, const FBaseRequest& BaseRequest, const FWriteToCellRequest& WriteToCellRequest, const FRequestFinishedDelegate& OnWriteToCellRequestFinished)
{
	WriteToCellRequestFinishedDelegate = OnWriteToCellRequestFinished;

	// Construct Url
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" + BaseRequest.SheetId + FString("/values/") +
		BaseRequest.Tab + FString("!") + WriteToCellRequest.CellToWriteTo + FString("?valueInputOption=RAW&access_token=");

	// Get parsed body
	FString body = OnRequestWriteToCellStarted(BaseRequest, WriteToCellRequest);

	FSpreadsheetsHttp::SendRequest(Credentials, url, FSpreadsheetsHttp::ERequestType::Put, FSpreadsheetsHttp::EReadWriteType::WriteToCell, body);
}

void USpreadsheetReadWrite::WriteToRange(const FSpreadsheetCrendentials& Credentials, const FBaseRequest& BaseRequest, const FWriteToRangeRequest& WriteToRangeRequest, const FRequestFinishedDelegate& OnWriteToRangeRequestFinished)
{
	WriteToRangeRequestFinishedDelegate = OnWriteToRangeRequestFinished;

	// Construct Url
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" + BaseRequest.SheetId + FString("/values/") +
		BaseRequest.Tab + FString("!") + WriteToRangeRequest.RangeLocationStart + FString(":") + WriteToRangeRequest.RangeLocationEnd +
		FString("?valueInputOption=RAW&access_token=");

	// Get parsed body
	FString body = OnRequestWriteToRangeStarted(BaseRequest, WriteToRangeRequest);

	FSpreadsheetsHttp::SendRequest(Credentials, url, FSpreadsheetsHttp::ERequestType::Put, FSpreadsheetsHttp::EReadWriteType::WriteToRange, body);
}

void USpreadsheetReadWrite::AppendToTable(const FSpreadsheetCrendentials& Credentials, const FBaseRequest& BaseRequest, const FAppendToTableRequest& AppendToTableRequest, const FRequestFinishedDelegate& OnAppendToTableRequestFinished)
{
	AppendToTableRequestFinishedDelegate = OnAppendToTableRequestFinished;

	// Construct Url
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" + BaseRequest.SheetId + FString("/values/") +
		BaseRequest.Tab + FString("!") + AppendToTableRequest.RangeLocationStart + FString(":") + AppendToTableRequest.RangeLocationEnd +
		FString(":append?valueInputOption=RAW&access_token=");

	// Get parsed body
	FString body = OnRequestAppendToTableStarted(BaseRequest, AppendToTableRequest);

	FSpreadsheetsHttp::SendRequest(Credentials, url, FSpreadsheetsHttp::ERequestType::Post, FSpreadsheetsHttp::EReadWriteType::AppendToTable, body);
}

void USpreadsheetReadWrite::ClearCell(const FSpreadsheetCrendentials& Credentials, const FBaseRequest& BaseRequest, const FClearCellRequest& ClearCellRequest, const FRequestFinishedDelegate& OnClearCellRequestFinished)
{
	ClearCellRequestFinishedDelegate = OnClearCellRequestFinished;

	// Construct Url
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
		BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
		FString("!") + ClearCellRequest.CellLocation + FString(":clear?access_token=");

	FSpreadsheetsHttp::SendRequest(Credentials, url, FSpreadsheetsHttp::ERequestType::Post, FSpreadsheetsHttp::EReadWriteType::ClearCell);
}

void USpreadsheetReadWrite::ClearRange(const FSpreadsheetCrendentials& Credentials, const FBaseRequest& BaseRequest, const FClearRangeRequest& ClearRangeRequest, const FRequestFinishedDelegate& OnClearRangeRequestFinished)
{
	ClearRangeRequestFinishedDelegate = OnClearRangeRequestFinished;

	// Construct Url
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
		BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
		FString("!") + ClearRangeRequest.RangeLocationStart + FString(":") + ClearRangeRequest.RangeLocationEnd + FString(":clear?access_token=");

	FSpreadsheetsHttp::SendRequest(Credentials, url, FSpreadsheetsHttp::ERequestType::Post, FSpreadsheetsHttp::EReadWriteType::ClearRange);
}

void USpreadsheetReadWrite::ExportSpreadsheet(const FSpreadsheetCrendentials& Credentials, const FBaseRequest& BaseRequest, const FExportRequest& ExportRequest, const FRequestFinishedDelegate& OnExportRequestFinished) {
	ExportRequestFinishedDelegate = OnExportRequestFinished;
	LocalExportRequest = ExportRequest;

	// Construct Url
	FString url = "https://sheets.googleapis.com/v4/spreadsheets/" +
		BaseRequest.SheetId + FString("/values/") + BaseRequest.Tab +
		FString("!") + ExportRequest.RangeLocationStart + FString(":") + ExportRequest.RangeLocationEnd + FString("?access_token=");

	FSpreadsheetsHttp::SendRequest(Credentials, url, FSpreadsheetsHttp::ERequestType::Get, FSpreadsheetsHttp::EReadWriteType::Export);
}

void USpreadsheetReadWrite::OnRequestReadCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	int responseCode = Response->GetResponseCode();

	FReadCellResponse response;

	if (ReadCellRequestFinishedDelegate.IsBound())
	{
		if (responseCode != 400 && responseCode != 401 && Success)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
			const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*Response->GetContentAsString());

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				auto doubleArrayResponse = JsonObject->GetArrayField("values");
				auto elementOneResponse = doubleArrayResponse[0]->AsArray();
				response.CellContent = elementOneResponse[0]->AsString();
			}

			ReadCellRequestFinishedDelegate.Execute(response, Success);
		}
		else
		{
			ReadCellRequestFinishedDelegate.Execute(response, false);
		}
	}
}

void USpreadsheetReadWrite::OnRequestReadRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	int responseCode = Response->GetResponseCode();

	FReadRangeResponse response;

	if (ReadRangeRequestFinishedDelegate.IsBound())
	{
		if (responseCode != 400 && responseCode != 401 && Success)
		{

			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*Response->GetContentAsString());

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				auto firstDimension = JsonObject->GetArrayField("values");

				for (auto dimensionElement : firstDimension)
				{
					auto dimensionElementAsArray = dimensionElement->AsArray();
					FSpreadsheetArray spreadsheetArray;

					for (auto arrayElement : dimensionElementAsArray)
					{
						spreadsheetArray.Elements.Add(arrayElement->AsString());
					}

					response.RangeContent.Add(spreadsheetArray);
				}
			}

			ReadRangeRequestFinishedDelegate.Execute(response, Success);
		}
		else
		{
			ReadRangeRequestFinishedDelegate.Execute(response, false);
		}
	}
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
	int responseCode = Response->GetResponseCode();

	if (WriteToCellRequestFinishedDelegate.IsBound())
	{
		if (responseCode != 400 && responseCode != 401 && Success)
		{
			WriteToCellRequestFinishedDelegate.Execute(Success);
		}
		else
		{
			WriteToCellRequestFinishedDelegate.Execute(false);
		}
	}
}

FString USpreadsheetReadWrite::OnRequestWriteToRangeStarted(const FBaseRequest& BaseRequest, const FWriteToRangeRequest& WriteToRangeRequest)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	const FString range = BaseRequest.Tab + FString("!") + WriteToRangeRequest.RangeLocationStart + FString(":") + WriteToRangeRequest.RangeLocationEnd;
	JsonObject->SetStringField("range", range);

	if (WriteToRangeRequest.Dimension == ESpreadsheetDimension::Columns)
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
	return FString(OutputString);
}

void USpreadsheetReadWrite::OnRequestWriteToRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	int responseCode = Response->GetResponseCode();

	if (WriteToRangeRequestFinishedDelegate.IsBound())
	{
		if (responseCode != 400 && responseCode != 401 && Success)
		{
			WriteToRangeRequestFinishedDelegate.Execute(Success);
		}
		else
		{
			WriteToRangeRequestFinishedDelegate.Execute(false);
		}
	}
}

FString USpreadsheetReadWrite::OnRequestAppendToTableStarted(const FBaseRequest& BaseRequest, const FAppendToTableRequest& WriteToRangeRequest)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	const FString range = BaseRequest.Tab + FString("!") + WriteToRangeRequest.RangeLocationStart + FString(":") + WriteToRangeRequest.RangeLocationEnd;
	JsonObject->SetStringField("range", range);

	if (WriteToRangeRequest.Dimension == ESpreadsheetDimension::Columns)
	{
		JsonObject->SetStringField("majorDimension", "COLUMNS");
	}
	else
	{
		JsonObject->SetStringField("majorDimension", "ROWS");
	}

	TArray<TSharedPtr<FJsonValue>> firstDimension;

	for (FSpreadsheetArray spreadsheetArray : WriteToRangeRequest.NewTableContent)
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
	return FString(OutputString);
}

void USpreadsheetReadWrite::OnRequestAppendToTableFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	int responseCode = Response->GetResponseCode();

	if (AppendToTableRequestFinishedDelegate.IsBound())
	{
		if (responseCode != 400 && responseCode != 401 && Success)
		{
			AppendToTableRequestFinishedDelegate.Execute(Success);
		}
		else
		{
			AppendToTableRequestFinishedDelegate.Execute(false);
		}
	}
}

void USpreadsheetReadWrite::OnRequestClearCellFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	int responseCode = Response->GetResponseCode();

	if (ClearCellRequestFinishedDelegate.IsBound())
	{
		if (responseCode != 400 && responseCode != 401 && Success)
		{
			ClearCellRequestFinishedDelegate.Execute(Success);
		}
		else
		{
			ClearCellRequestFinishedDelegate.Execute(false);
		}
	}
}

void USpreadsheetReadWrite::OnRequestClearRangeFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	int responseCode = Response->GetResponseCode();

	if (ClearRangeRequestFinishedDelegate.IsBound())
	{
		if (responseCode != 400 && responseCode != 401 && Success)
		{
			ClearRangeRequestFinishedDelegate.Execute(Success);
		}
		else
		{
			ClearRangeRequestFinishedDelegate.Execute(false);
		}
	}
}

void USpreadsheetReadWrite::OnRequestExportFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	int responseCode = Response->GetResponseCode();

	if (ExportRequestFinishedDelegate.IsBound())
	{
		if (responseCode != 400 && responseCode != 401 && Success)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*Response->GetContentAsString());

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				auto doubleArrayResponse = JsonObject->GetArrayField("values");
				FString content = "";

				for (auto arr : doubleArrayResponse)
				{
					auto elResponse = arr->AsArray();
					for (int i = 0; i < elResponse.Num(); ++i)
					{
						FString element = elResponse[i]->AsString();

						if (i != 0)
						{
							content += FString(",");
						}

						if (element.Contains(","))
						{
							content += FString("\"") + element += FString("\"");
						}
						else
						{
							content += element;
						}
					}
					content += FString("\n");
				}

				FString path = FPaths::ProjectContentDir() + LocalExportRequest.OutputDestination;

				FFileHelper::SaveStringToFile(content, *path);
			}

			ExportRequestFinishedDelegate.Execute(Success);
		}
		else
		{
			ExportRequestFinishedDelegate.Execute(false);
		}
	}
}