// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/CredentialSettings.h"

#if WITH_EDITOR
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "LevelEditor.h"
#include "SettingsHelper.h"
#endif

#include "Settings/ProjectPackagingSettings.h"

#define LOCTEXT_NAMESPACE "UserSettingsMenu"
UCredentialSettings::UCredentialSettings(const FObjectInitializer &ObjectInitializer): Super(ObjectInitializer)
{
}

TSharedRef<IDetailCustomization> FSpreadsheetsSettingsDetails::MakeInstance()
{
    return MakeShareable(new FSpreadsheetsSettingsDetails);
}

void FSpreadsheetsSettingsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
#if WITH_EDITOR
    TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
    DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);
    check(ObjectsBeingCustomized.Num() == 1);
    TWeakObjectPtr<UCredentialSettings> Settings = Cast<UCredentialSettings>(ObjectsBeingCustomized[0].Get());

    IDetailCategoryBuilder& EncryptionCategory = DetailBuilder.EditCategory("Spreadsheets With Unreal");
    //UProjectPackagingSettings* ProjectPackagingSettings = GetMutableDefault<UProjectPackagingSettings>();
    EncryptionCategory.AddCustomRow(LOCTEXT("SaveOAuthpaths", "SaveOAuthpaths"))
        .ValueContent()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .Padding(5)
            .AutoWidth()
            [
                SNew(SButton)
                .Text(LOCTEXT("SaveOAuthpaths", "Save OAuth paths"))
                .ToolTipText(LOCTEXT("SaveOAuthpaths_Tooltip", "Saves all in credential options array stored OAuth paths to the additional Non-Asset Directories to Package"))
                .OnClicked_Lambda([this, Settings]()
                    {
                        for (auto it = Settings->CredentialOptions.begin(); it != Settings->CredentialOptions.end(); ++it) {

                            if(!SettingsHelper::ExistsInExcludeDirectories(it->Value.PathToOAuthCredentialsFile))
                            {
                                SettingsHelper::AddToProjectExcludeDirectories(it->Value.PathToOAuthCredentialsFile);
                            }
                        }


                        return(FReply::Handled());
                    })
            ]
        
        ];
#endif
}

#undef LOCTEXT_NAMESPACE