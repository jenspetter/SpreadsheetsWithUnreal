// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Http.h"
#include "HttpModule.h"
#include "Requests/Request.h"
#include "UObject/NoExportTypes.h"
#include "Response.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct SPREADSHEETSWITHUNREAL_API FReadCellResponse {
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Response")
    FString CellContent;
};

USTRUCT(BlueprintType, Blueprintable)
struct SPREADSHEETSWITHUNREAL_API FReadRangeResponse {
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Response")
    TArray<FSpreadsheetArray> RangeContent;
};