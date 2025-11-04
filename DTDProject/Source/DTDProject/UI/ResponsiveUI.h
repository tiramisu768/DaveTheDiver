// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResponsiveUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UResponsiveUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta =(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> ShowAnim;

	UPROPERTY(meta =(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> HideAnim;

	FTimerHandle SustainTimerHandle;

public:
	UFUNCTION(BlueprintCallable, Category = "Responsive UI")
	virtual void ShowUIAnim(float SustainTime = -1.0f);

	UFUNCTION(BlueprintCallable, Category = "Responsive UI")
	virtual void HideUIAnim();
};
