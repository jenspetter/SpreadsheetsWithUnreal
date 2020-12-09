// Fill out your copyright notice in the Description page of Project Settings.


#include "Responses/Response.h"

void UBaseResponse::OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success)
{
	/*if (success)
	{
		FString result = response->GetContentAsString();

		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Orange, FString::Printf(TEXT("%s"), *result));
		UE_LOG(LogTemp, Warning, TEXT("%s"), "Good request");
		UE_LOG(LogTemp, Warning, TEXT("%s"), *request->GetResponse()->GetContentAsString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), "Not good request");
		GEngine->AddOnScreenDebugMessage(0, 4.0f, FColor::Emerald, TEXT("The message wasnt received"));
	}*/

	if (success)
	{
		m_RequestSuccessful = true;
	}
	else
	{
		m_RequestSuccessful = false;
	}



	//m_Del.Execute(this, m_RequestSuccessful);
}

void UTokenResponse::OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success)
{
}

FString UReadCellResponse::GetCell()
{
	return m_Cell;
}

void UReadCellResponse::OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success)
{
	if (success)
	{
		m_RequestSuccessful = true;

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*response->GetContentAsString());

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			auto doubleArrayResponse = JsonObject->GetArrayField("values");
			auto elementOneResponse = doubleArrayResponse[0]->AsArray();
			m_Cell = elementOneResponse[0]->AsString();
		}
	}
	else
	{
		m_RequestSuccessful = false;
	}

	m_RequestFinishedDelegate.Execute(this, m_RequestSuccessful);
}

void UReadRangeResponse::OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success)
{
	if (success)
	{
		m_RequestSuccessful = true;

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*response->GetContentAsString());

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			auto doubleArrayResponse = JsonObject->GetArrayField("values");

			for (auto arr : doubleArrayResponse)
			{
				auto elResponse = arr->AsArray();
				for (auto el : elResponse)
				{
					m_Range.Add(el->AsString());
				}
			}
		}
	}
	else
	{
		m_RequestSuccessful = false;
	}

	m_RequestFinishedDelegate.Execute(this, m_RequestSuccessful);
}

void UExportResponse::SetExportData(UDataTable* a_DataTable, TEnumAsByte<EExportFormat> a_ExportFormat, FString a_OutputDestination)
{
	m_DataTable = a_DataTable;
	m_ExportFormat = a_ExportFormat;
	m_OutputDestination = a_OutputDestination;
}

void UExportResponse::OnRequestFinished(FHttpRequestPtr request, FHttpResponsePtr response, bool success)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Response"));
	if (success)
	{
		m_RequestSuccessful = true;

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(*response->GetContentAsString());

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			auto doubleArrayResponse = JsonObject->GetArrayField("values");
			FString content = "";

			for (auto arr : doubleArrayResponse)
			{
				auto elResponse = arr->AsArray();
				for(int i = 0; i < elResponse.Num(); ++i)
				{
					FString element = elResponse[i]->AsString();

					if(i != 0)
					{
						content += FString(",");
					}

					if(element.Contains(","))
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

			UE_LOG(LogTemp, Warning, TEXT("%s"), *content);

			FString path = FPaths::ProjectContentDir() + FString("MyTestStrings/Test1.csv");

			FFileHelper::SaveStringToFile(content, *path);

			FString FileContent;

			//Read the csv file
			FFileHelper::LoadFileToString(FileContent, *path);

			//If data table is valid that is contained in class
			if (m_DataTable)
			{
				//Array of exported rows of csv file
				///Also load in data from csv into data table
				TArray<FString> problems = m_DataTable->CreateTableFromCSVString(FileContent);

				if (problems.Num() > 0)
				{
					for (int32 ProbIdx = 0; ProbIdx < problems.Num(); ProbIdx++)
					{
						//Erros detected
					}
					UE_LOG(LogTemp, Warning, TEXT("Data Table update failed"));
				}
				else
				{
					//Updated Successfully
					UE_LOG(LogTemp, Warning, TEXT("Data Table updated successfully"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Data Table not found"));
			}
		}
	}
	else
	{
		m_RequestSuccessful = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("End Response"));
	m_RequestFinishedDelegate.Execute(m_RequestSuccessful);
}
