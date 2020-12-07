// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Request.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBaseRequest
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Sheet Token", Category = "Spreadsheets | Request")
	FString m_Token;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Sheet ID", Category = "Spreadsheets | Request")
	FString m_SheetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Tab", Category = "Spreadsheets | Request")
	FString m_Tab;
};

USTRUCT(BlueprintType)
struct FReadCellRequest
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, DisplayName = "Cell", Category = "Spreadsheets | Request")
	FString m_Cell;
};

USTRUCT(BlueprintType)
struct FReadRangeRequest
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, DisplayName = "Range", Category = "Spreadsheets | Request")
	FString m_Range;
};