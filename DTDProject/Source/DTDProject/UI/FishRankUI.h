// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ResponsiveUI.h"
#include "ActorComponent/InventoryComponent.h"
#include "FishRankUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UFishRankUI : public UResponsiveUI
{
	GENERATED_BODY()
private:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_FishName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_FishGrade;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_FishWeight;
public:
	void UpdateAndShow(const FCaughtFishInfo& FishInfo,float SustainTime);
};
