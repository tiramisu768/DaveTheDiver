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
	TObjectPtr<class UImage> Image_Arrow;

	FVector2D ArcCenter = FVector2D(0, 0);
	float ArcRadius = 150.f;
public:
	void ResetAimPos();
	void NativeConstruct();
	void SetArcInfo(FVector2D Center, float Radius) { ArcCenter = Center; ArcRadius = Radius; }
	void UpdateAimPos(FVector2D AimPos);
	FVector2D GetArcCenter() const { return ArcCenter; }
	float GetArcRadius() const { return ArcRadius; }
};
