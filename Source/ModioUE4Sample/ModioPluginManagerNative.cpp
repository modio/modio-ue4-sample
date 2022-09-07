// Fill out your copyright notice in the Description page of Project Settings.

#include "ModioPluginManagerNative.h"
#include "Libraries/ModioSDKLibrary.h"
#include "ModioSubsystem.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioInitializeOptions.h"

// Sets default values
AModioPluginManagerNative::AModioPluginManagerNative()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AModioPluginManagerNative::BeginPlay()
{
	Super::BeginPlay();
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		FModioInitializeOptions Options = UModioSDKLibrary::GetProjectInitializeOptions();
		Options.LocalSessionIdentifier = "LocalPlayer";
		Subsystem->InitializeAsync(Options, FOnErrorOnlyDelegateFast::CreateUObject(
												this, &AModioPluginManagerNative::OnInitializationComplete));
	}
}

void AModioPluginManagerNative::EndPlay(EEndPlayReason::Type)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ShutdownAsync(FOnErrorOnlyDelegateFast::CreateLambda([](FModioErrorCode ec) {}));
	}
}

void AModioPluginManagerNative::OnInitializationComplete(FModioErrorCode ec)
{
	if (!ec)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			Subsystem->VerifyUserAuthenticationAsync(
				FOnErrorOnlyDelegateFast::CreateUObject(this, &AModioPluginManagerNative::OnVerifyAuthComplete));
		}
	}
}

void AModioPluginManagerNative::OnVerifyAuthComplete(FModioErrorCode ec)
{
	if (!ec)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (!Subsystem->EnableModManagement(FOnModManagementDelegateFast::CreateUObject(
					this, &AModioPluginManagerNative::OnModManagementEvent)))
			{
				Subsystem->FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast::CreateLambda([](FModioErrorCode) {}));
				OnShowModBrowser.Broadcast();
			}
		}
	}
	else
	{
		OnShowAuthenticationDialog.Broadcast();
	}
}

void AModioPluginManagerNative::OnModManagementEvent(FModioModManagementEvent NewEvent) {}

// Called every frame
void AModioPluginManagerNative::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->RunPendingHandlers();
	}
}
