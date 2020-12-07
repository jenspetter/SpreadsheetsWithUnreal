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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "PathToOAuthCredentialsFile", Category = "JSheets | Credentials")
	FString m_PathToOAuthCredentialsFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "RefreshToken", Category = "JSheets | Credentials")
	FString m_RefreshToken;
};

UCLASS(config = Engine, defaultconfig)
class SPREADSHEETSWITHUNREAL_API UCredentialSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(config, EditAnywhere, DisplayName = "CredentialOptions", Category = Settings)
	TArray<struct FSpreadsheetCrendentials> m_CredentialOptions;
};
