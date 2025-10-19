// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboAimUI.h"
#include "Components/Overlay.h"

void URoboAimUI::SetAimPos(float Pos)
{
	if (Overlay_Arrow)
    {
        FVector2D ArrowPos = ArcCenter + FVector2D(FMath::Cos(Pos), FMath::Sin(Pos)) * ArcRadius;
        Overlay_Arrow->SetRenderTranslation(ArrowPos - GetCachedGeometry().GetLocalSize() * 0.5f);

        // 위치 로그 출력
        UE_LOG(LogTemp, Warning, TEXT("Overlay_Arrow Pos: X=%f, Y=%f"), ArrowPos.X, ArrowPos.Y);
    }
}