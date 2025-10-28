// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FishRankUI.h"

void UFishRankUI::ShowRank()
{
	if (SlideInOutAnim)
	{
		PlayAnimation(SlideInOutAnim);
	}
}
