// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Request.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum EExportFormat
{
    CSV
};

UENUM(BlueprintType)
enum ESpreadsheetDimension
{
    ROWS,
    COLUMNS
};

USTRUCT(BlueprintType)
struct FSpreadsheetArray
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    TArray<FString> Elements;
};

USTRUCT(BlueprintType)
struct FBaseRequest {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString SheetId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString Tab;
};

USTRUCT(BlueprintType)
struct FReadCellRequest {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString Cell;
};

USTRUCT(BlueprintType)
struct FReadRangeRequest {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString Range;
};

USTRUCT(BlueprintType)
struct FWriteToCellRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString CellToWriteTo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString NewCellContent;
};

USTRUCT(BlueprintType)
struct FWriteToRangeRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString RangeToWriteTo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    TEnumAsByte<ESpreadsheetDimension> Dimension;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    TArray<FSpreadsheetArray> NewRangeContent;
};

USTRUCT(BlueprintType)
struct FClearCellRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString Cell;
};

USTRUCT(BlueprintType, DisplayName = "Clear Range Request")
struct FClearRangeRequest
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString Range;
};

USTRUCT(BlueprintType)
struct FExportRequest {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    TEnumAsByte<EExportFormat> ExportFormat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString Range;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Request")
    FString OutputDestination;
};