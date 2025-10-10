// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboWeaponUI.h"

void URoboWeaponUI::PlaySwitchAnimation(int32 SelectedIndex)
{
	if (SelectedIndex == 0)
	{
		PlayAnimation(TabAni);
	}
	else if (SelectedIndex == 1)
	{
		PlayAnimation(TabAni_Reverse);
	}
}
