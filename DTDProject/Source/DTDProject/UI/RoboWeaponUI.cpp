// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboWeaponUI.h"
#include "MainUI.h"

void URoboWeaponUI::PlaySwitchAnimation(int32 SelectedIndex)
{
	if (SelectedIndex == 0)
	{
		PlayAnimation(RangedAnim);
	}
	else if (SelectedIndex == 1)
	{
		PlayAnimation(RangedAnim_Reverse);
	}
}
