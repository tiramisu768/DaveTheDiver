// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoboAimUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API URoboAimUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Img_Arrow;
public:
	void SetAimAngle(float Angle);
	
};
