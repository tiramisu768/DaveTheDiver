// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LongPressUI.h"
#include "LongPressUI.h"
#include "Components/ProgressBar.h"

void ULongPressUI::SetLongPressBarPercent(float Percent)
{
	if (LongPressBar)
		LongPressBar->SetPercent(Percent);
}
