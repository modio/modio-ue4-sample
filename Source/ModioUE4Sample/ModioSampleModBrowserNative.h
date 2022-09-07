// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Misc/Optional.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModInfoList.h"
#include "ModioSubsystem.h"

#include "ModioSampleModBrowserNative.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRefreshBrowser, FModioErrorCode, ErrorCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyListAllModsCompleted, FModioErrorCode, ErrorCode, FModioOptionalModInfoList,
								   Result);
/**
 *
 */
UCLASS()
class MODIOUE4SAMPLE_API UModioSampleModBrowserNative : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "mod.io Sample")
	class UEditableTextBox* SearchInputField;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "mod.io Sample")
	class UUserWidget* SubmitButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "mod.io Sample")
	class UListView* SearchResultList;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "mod.io Sample")
	class UUserWidget* StartGameButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "mod.io Sample")
	class UComboBoxString* EnabledModChoice;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget), Category = "mod.io Sample")
	class URichTextBlock* SelectedModLabel;


	UPROPERTY(BlueprintAssignable)
	FOnRefreshBrowser OnRefreshBrowser;

	UPROPERTY(BlueprintAssignable)
	FNotifyListAllModsCompleted NotifyListAllModsCompleted;

	void Refresh();

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable, Category = "mod.io Samples")
	void OnModSubscriptionButtonClicked(FModioModInfo AssociatedMod, bool bNewSubscriptionValue);

	void OnSubscriptionChanged(FModioErrorCode ec);
	void OnExternalUpdatesFetched(FModioErrorCode ec);

	void OnListModsCompleted(FModioErrorCode ec, TOptional<FModioModInfoList> MaybeModList);
};
