// Fill out your copyright notice in the Description page of Project Settings.

#include "ModioSampleModBrowserNative.h"
#include "Components/EditableTextBox.h"
#include "ModioSubsystem.h"
#include "Types/ModioFilterParams.h"
#include "Types/ModioModInfoList.h"

void UModioSampleModBrowserNative::Refresh()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		FModioFilterParams Params;
		if (SearchInputField)
		{
			Params.NameContains(SearchInputField->GetText().ToString());
		}
		Subsystem->ListAllModsAsync(Params, FOnListAllModsDelegateFast::CreateUObject(
												this, &UModioSampleModBrowserNative::OnListModsCompleted));
	}
}

void UModioSampleModBrowserNative::NativeConstruct()
{
	Super::NativeConstruct();
	Refresh();
}

void UModioSampleModBrowserNative::NativeOnInitialized() 
{

}

void UModioSampleModBrowserNative::OnModSubscriptionButtonClicked(FModioModInfo AssociatedMod,
																  bool bNewSubscriptionValue)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (bNewSubscriptionValue)
		{
			Subsystem->SubscribeToModAsync(
				AssociatedMod.ModId,
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioSampleModBrowserNative::OnSubscriptionChanged));
		}
		else
		{
			Subsystem->UnsubscribeFromModAsync(
				AssociatedMod.ModId,
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioSampleModBrowserNative::OnSubscriptionChanged));
		}
	}
}

void UModioSampleModBrowserNative::OnSubscriptionChanged(FModioErrorCode ec)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->FetchExternalUpdatesAsync(
			FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioSampleModBrowserNative::OnExternalUpdatesFetched));
	}
}

void UModioSampleModBrowserNative::OnExternalUpdatesFetched(FModioErrorCode ec)
{
	OnRefreshBrowser.Broadcast(ec);
}

void UModioSampleModBrowserNative::OnListModsCompleted(FModioErrorCode ec, TOptional<FModioModInfoList> MaybeModList)
{
	NotifyListAllModsCompleted.Broadcast(ec, FModioOptionalModInfoList(MoveTemp(MaybeModList)));
}
