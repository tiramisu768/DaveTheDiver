// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ResponsiveUI.h"
#include "ActorComponent/InventoryComponent.h"
#include "FishRankUI.generated.h"

UCLASS()
class DTDPROJECT_API UFishRankUI : public UResponsiveUI
{
	GENERATED_BODY()
public:
	void ShowFishCollected(const FCaughtFishInfo& FishInfo, float SustainTime);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> FishIcon;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> FishNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> FishGradeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> FishWeightText;
};
