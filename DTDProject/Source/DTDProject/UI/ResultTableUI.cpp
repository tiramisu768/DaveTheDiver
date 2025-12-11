// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultTableUI.h"
#include "Components/WidgetSwitcher.h"

void UResultTableUI::SetGameEnd(bool bWasSuccessful)
{
	if (ResultSwitcher)
	{
		if (bWasSuccessful)
		{
			ResultSwitcher->SetActiveWidgetIndex(GoodResultIndex);
		}
		else
		{
			ResultSwitcher->SetActiveWidgetIndex(BadResultIndex);
		}
	}
//잡은 물고기 목록을 채우는 등의 공통 로직을 추가할 수 있음
}
