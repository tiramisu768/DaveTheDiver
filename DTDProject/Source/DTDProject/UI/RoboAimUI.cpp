// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboAimUI.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void URoboAimUI::NativeConstruct()
{
    Super::NativeConstruct();

    FVector2D ViewportSize;

    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);

        ArcCenter = ViewportSize * 0.5f;
        if (Image_Arrow)
        {
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Arrow->Slot);
            ArcCenter = CanvasSlot->GetPosition();
        }
    }
}

void URoboAimUI::ResetAimPos()
{
    if (Image_Arrow)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Arrow->Slot);
        CanvasSlot->SetPosition(ArcCenter);
    }
}

void URoboAimUI::UpdateAimPos(FVector2D AimPos)
{
    if (Image_Arrow)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Arrow->Slot);
        if (CanvasSlot)
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + AimPos);
            FVector2D Direction = CanvasSlot->GetPosition() - ArcCenter;
            if (Direction.Length() > ArcRadius)
            {
                Direction.Normalize();
                CanvasSlot->SetPosition(ArcCenter + Direction * ArcRadius);
            }
            // 위치 로그 출력
           /* UE_LOG(LogTemp, Warning, TEXT("Overlay_Arrow Pos: X=%f, Y=%f"), AimPos.X, AimPos.Y);*/
        }
    }
}