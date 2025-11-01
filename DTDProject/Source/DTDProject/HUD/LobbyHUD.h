// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
private:
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UUserWidget* LobbyWidgetInstance;

public:
	ALobbyHUD();
	virtual void BeginPlay() override;
};
