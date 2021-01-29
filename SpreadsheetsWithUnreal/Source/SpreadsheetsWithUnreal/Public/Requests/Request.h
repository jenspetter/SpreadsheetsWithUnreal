// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Request.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EExportFormat : uint8
{
    Csv
};

UENUM(BlueprintType)
enum class ESpreadsheetDimension : uint8
{
    Rows,
    Columns
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FSpreadsheetArray
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    TArray<FString> Elements;
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FBaseRequest {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString SheetId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString Tab;
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FReadCellRequest {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString CellLocation;
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FReadRangeRequest {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationStart;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationEnd;
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FWriteToCellRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString CellToWriteTo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString NewCellContent;
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FWriteToRangeRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationStart;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationEnd;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    ESpreadsheetDimension Dimension;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    TArray<FSpreadsheetArray> NewRangeContent;
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FAppendToTableRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationStart;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationEnd;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    ESpreadsheetDimension Dimension;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    TArray<FSpreadsheetArray> NewTableContent;
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FClearCellRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString CellLocation;
};

USTRUCT(BlueprintType, DisplayName = "Clear Range Request")
struct SPREADSHEETSWITHUNREAL_API FClearRangeRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationStart;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationEnd;
};

USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FExportRequest {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    EExportFormat ExportFormat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationStart;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeLocationEnd;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString OutputDestination;
};