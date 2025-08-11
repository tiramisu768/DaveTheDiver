// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()
private:
	TSubclassOf<UUserWidget> HPBarWidget;
	TObjectPtr<class URoboHPBarUI> RoboHPBarUIClass;

public:
	AMyHUD();
	void BeginPlay() override;
	void SetHPPercent(float value);
};
