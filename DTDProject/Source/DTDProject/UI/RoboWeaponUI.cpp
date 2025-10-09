// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboWeaponUI.h"

void URoboWeaponUI::PlaySwitchAnimation(int32 SelectedIndex)
{
	for (int32 i = 0; i < 2; ++i)
	{
		if (i == SelectedIndex)
		{
			WeaponOverlayArray[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			WeaponOverlayArray[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
