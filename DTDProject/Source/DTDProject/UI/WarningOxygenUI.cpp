// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WarningOxygenUI.h"

void UWarningOxygenUI::SetOxygenWarning()
{
	if (BlinkRedScreenAnim)
	{
		PlayAnimation(BlinkRedScreenAnim, 0.f, 0);
	}
}

void UWarningOxygenUI::StopOxygenWarning()
{
	if (BlinkRedScreenAnim)
	{
		StopAnimation(BlinkRedScreenAnim);
	}
}
