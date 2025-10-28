// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorComponent/InventoryComponent.h"
#include "FishRankUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UFishRankUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta =(BindWidgetAnim),Transient)
	UWidgetAnimation* SlideInOutAnim;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_FishName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_FishGrade;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_FishWeight;
public:
	void ShowRank();
	void UpdateFishRankList(const TArray<FCaughtFishInfo>& FishList);
};
