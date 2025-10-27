// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarningOxygenUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UWarningOxygenUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BlinkRedScreenAnim;

public:
	void SetOxygenWarning();
	void StopOxygenWarning();
};
