// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResponsiveUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUIHidden);

UCLASS()
class DTDPROJECT_API UResponsiveUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Responsive UI")
	virtual void ShowUI();

	UFUNCTION(BlueprintCallable, Category = "Responsive UI")
	virtual void ShowNotification(float SustainTime);

	UFUNCTION(BlueprintCallable, Category = "Responsive UI")
	virtual void HideUI();

	UPROPERTY(BlueprintAssignable, Category = "Responsive UI")
	FOnUIHidden OnHidden;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Responsive UI")
	void OnShow();
	UFUNCTION(BlueprintImplementableEvent, Category = "Responsive UI")
	void OnHide();
	UFUNCTION()
	void OnHideAnimationFinished();

	UPROPERTY(meta =(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> ShowAnim;

	UPROPERTY(meta =(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> HideAnim;

	FTimerHandle SustainTimerHandle;


};
