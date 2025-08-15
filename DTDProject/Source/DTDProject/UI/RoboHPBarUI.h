// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoboHPBarUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API URoboHPBarUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> RoboHPBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RoboHPText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RoboDepthText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RoboDepthTitleText;
public:
	void SetHPBarPercent(float Percent);
};
