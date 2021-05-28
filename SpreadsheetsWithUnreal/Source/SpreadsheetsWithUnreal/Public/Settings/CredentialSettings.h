// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"

#include "CredentialSettings.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct SPREADSHEETSWITHUNREAL_API FSpreadsheetCrendentials
{
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Credentials")
    FString PathToOAuthCredentialsFile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spreadsheets With Unreal | Credentials")
    FString RefreshToken;
};

UCLASS(config = Game, defaultconfig)
class SPREADSHEETSWITHUNREAL_API UCredentialSettings : public UObject
{
    GENERATED_UCLASS_BODY()

    UPROPERTY(config, EditAnywhere, Category = "Spreadsheets With Unreal", meta = (ToolTip = "Credentials that are needed in order to call Google REST API calls. The downloaded OAuth from the Google Developer console from Google and a refresh token generated in the OAuth playground from Google are needed."))
    TMap<FString, struct FSpreadsheetCrendentials> CredentialOptions;
};

class FSpreadsheetsSettingsDetails : public IDetailCustomization
{
public:
    /** Makes a new instance of this detail layout class for a specific detail view requesting it */
    static TSharedRef<IDetailCustomization> MakeInstance();

    /** IDetailCustomization interface */
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};