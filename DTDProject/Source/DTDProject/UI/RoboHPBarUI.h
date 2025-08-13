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
	TObjectPtr<class UProgressBar> RoboProgressBar;
	TObjectPtr<class UTextBlock> RoboHPText;
public:
	void SetHPBarPercent(float Percent);
};
