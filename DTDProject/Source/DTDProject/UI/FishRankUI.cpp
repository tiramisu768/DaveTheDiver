// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FishRankUI.h"
#include "Components/TextBlock.h"

void UFishRankUI::ShowRank()
{
	if (SlideInOutAnim)
	{
		PlayAnimation(SlideInOutAnim);
	}
}

void UFishRankUI::UpdateFishRankList(const TArray<FCaughtFishInfo>& FishList)
{
	if (Text_FishName && FishList.Num()>0)
	{
		Text_FishName->SetText(FText::FromString(FishList.Last().FishName));
	}
	if (Text_FishGrade && FishList.Num() > 0)
	{
		Text_FishGrade->SetText(FText::FromString(FString::Printf(TEXT("%d"), FishList.Last().Grade)));
	}
	if (Text_FishWeight && FishList.Num() > 0)
	{
		Text_FishWeight->SetText(FText::FromString(FString::Printf(TEXT("%.2f kg"), FishList.Last().Weight)));
	}

}
