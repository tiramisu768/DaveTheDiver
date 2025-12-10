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
	TObjectPtr<class UProgressBar> HPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HPText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SeaDepthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InventoryCurrentTotalWeightText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InventoryMaxWeightText;
public:
	void SetHPBarPercent(float Percent);
	void SetCurrentDepthMeters(float Meter);
	void SetCurrentAndMaxWeight(float Current, float Max);
	void ShowOverWeight(bool becameOverweight);
};
