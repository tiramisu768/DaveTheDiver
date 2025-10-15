// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboAimUI.h"
#include "Components/Overlay.h"

void URoboAimUI::SetAimAngle(float Angle)
{
	if (Overlay_Arrow)
	{
		float Degrees = FMath::RadiansToDegrees(Angle);
		FVector2D ArrowPos = ArcCenter + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * ArcRadius;
		Overlay_Arrow->SetRenderTranslation(ArrowPos - GetCachedGeometry().GetLocalSize() * 0.5f);
		Overlay_Arrow->SetRenderTransformAngle(Degrees);
	}
}