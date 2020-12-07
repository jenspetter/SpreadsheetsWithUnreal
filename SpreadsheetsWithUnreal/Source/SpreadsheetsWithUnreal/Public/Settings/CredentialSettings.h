// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CredentialSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSpreadsheetCrendentials
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "PathToOAuthCredentialsFile", Category = "Spreadsheets With Unreal | Credentials")
	FString m_PathToOAuthCredentialsFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "RefreshToken", Category = "Spreadsheets With Unreal | Credentials")
	FString m_RefreshToken;
};

UCLASS(config = Engine, defaultconfig)
class SPREADSHEETSWITHUNREAL_API UCredentialSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(config, EditAnywhere, DisplayName = "CredentialOptions", Category = Settings)
	TMap<FString, struct FSpreadsheetCrendentials> m_CredentialOptions;
};
