// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleDamageUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API USimpleDamageUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageText;
public:
	void SetDamageText(float DamageAmount);
};
