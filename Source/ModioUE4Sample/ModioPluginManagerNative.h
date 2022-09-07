// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModManagementEvent.h"
#include "Delegates/Delegate.h"

#include "ModioPluginManagerNative.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleDynamicMulticastDelegate);

UCLASS()
class MODIOUE4SAMPLE_API AModioPluginManagerNative : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AModioPluginManagerNative();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type) override;
	void OnInitializationComplete(FModioErrorCode ec);

	void OnVerifyAuthComplete(FModioErrorCode ec);

	void OnModManagementEvent(FModioModManagementEvent NewEvent);

	UPROPERTY(BlueprintAssignable, Category = "mod.io Sample")
	FSimpleDynamicMulticastDelegate OnShowAuthenticationDialog;

	UPROPERTY(BlueprintAssignable, Category = "mod.io Sample")
	FSimpleDynamicMulticastDelegate OnShowModBrowser;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
