// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SimpleDamageUI.h"
#include "Components/TextBlock.h"

void USimpleDamageUI::SetDamageText(float DamageAmount)
{
	if (DamageText)
	{
		DamageText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), DamageAmount)));
	}
}
