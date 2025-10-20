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

        //FGeometry Geometry = GetCachedGeometry();
        //ArcCenter = Geometry.GetAbsolutePosition();
        ArcCenter = ViewportSize * 0.5f;
        if (Overlay_Arrow)
        {
            UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Overlay_Arrow->Slot);
            ArcCenter = CanvasSlot->GetPosition();
        }
    }
}

void URoboAimUI::ResetPosition()
{
    if (Overlay_Arrow)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Overlay_Arrow->Slot);
        CanvasSlot->SetPosition(ArcCenter);
    }
}

void URoboAimUI::SetAimPos(FVector2D AimPos)
{
    if (Overlay_Arrow)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Overlay_Arrow->Slot);
        if (CanvasSlot)
        {
            CanvasSlot->SetPosition(CanvasSlot->GetPosition() + AimPos);
            FVector2D Direction = CanvasSlot->GetPosition() - ArcCenter;
            if (Direction.Length() > 150.0)
            {
                Direction.Normalize();
                CanvasSlot->SetPosition(ArcCenter + Direction * 150.0f);
            }
            //Overlay_Arrow->SetRenderTranslation(AimPos /*- GetCachedGeometry().GetLocalSize() * 0.5f*/);

            // 위치 로그 출력
            UE_LOG(LogTemp, Warning, TEXT("Overlay_Arrow Pos: X=%f, Y=%f"), AimPos.X, AimPos.Y);
        }
    }
}