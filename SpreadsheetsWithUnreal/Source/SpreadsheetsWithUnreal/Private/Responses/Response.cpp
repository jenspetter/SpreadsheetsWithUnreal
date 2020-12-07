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