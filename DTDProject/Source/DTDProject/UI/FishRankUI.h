// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FishRankUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UFishRankUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta =(BindWidgetAnim),Transient)
	UWidgetAnimation* SlideInOutAnim;

public:
	void ShowRank();

};
