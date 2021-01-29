// Fill out your copyright notice in the Description page of Project Settings.


#include "SpreadsheetsFileSystem.h"

USpreadsheetsFileSystem::USpreadsheetsFileSystem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void USpreadsheetsFileSystem::ImportCSVToDataTable(const FString& PathToCSVFile, UDataTable* DataTable)
{
	FString FileContent;

	FString path = FPaths::ProjectContentDir() + PathToCSVFile;
	FFileHelper::LoadFileToString(FileContent, *path);

	if (DataTable) {
		TArray<FString> problems = DataTable->CreateTableFromCSVString(FileContent);

		if (problems.Num() > 0) {
			for (int32 ProbIdx = 0; ProbIdx < problems.Num(); ProbIdx++) {
				// Errors detected
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

FSpreadsheetCrendentials USpreadsheetsFileSystem::GetSpreadsheetCredentials(const FString& CredentialName) {
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
