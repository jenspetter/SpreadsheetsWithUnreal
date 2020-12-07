// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Request.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SPREADSHEETSWITHUNREAL_API UBaseRequest : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Sheet Token", Category = "Spreadsheets | Request")
		FString m_Token;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Sheet ID", Category = "Spreadsheets | Request")
		FString m_SheetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Tab", Category = "Spreadsheets | Request")
		FString m_Tab;
};

UCLASS(BlueprintType, Blueprintable)
class SPREADSHEETSWITHUNREAL_API UReadCellRequest : public UBaseRequest
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, DisplayName = "Cell", Category = "Spreadsheets | Request")
		FString m_Cell;
};

UCLASS(BlueprintType, Blueprintable)
class SPREADSHEETSWITHUNREAL_API UReadRangeRequest : public UBaseRequest
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, DisplayName = "Range", Category = "Spreadsheets | Request")
		FString m_Range;
};