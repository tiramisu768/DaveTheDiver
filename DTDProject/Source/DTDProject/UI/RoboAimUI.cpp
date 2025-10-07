// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboAimUI.h"
#include "Components/Image.h"

void URoboAimUI::SetAimAngle(float Angle)
{
	if (Img_Arrow)
	{
		float Degrees = FMath::RadiansToDegrees(Angle);
		Img_Arrow->SetRenderTransformAngle(Degrees);
}
}