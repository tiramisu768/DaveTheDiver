// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboAimUI.h"
#include "Components/Overlay.h"

void URoboAimUI::NativeConstruct()
{
    Super::NativeConstruct();
    FVector2D ViewportSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
        ArcCenter = ViewportSize * 0.5f;
    }
}

void URoboAimUI::SetAimPos(FVector2D AimPos)
{
	if (Overlay_Arrow)
    {
        Overlay_Arrow->SetRenderTranslation(AimPos - GetCachedGeometry().GetLocalSize() * 0.5f);

        // 위치 로그 출력
        UE_LOG(LogTemp, Warning, TEXT("Overlay_Arrow Pos: X=%f, Y=%f"), AimPos.X, AimPos.Y);
    }
}