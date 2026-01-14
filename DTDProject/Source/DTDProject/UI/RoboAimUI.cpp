// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RoboAimUI.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"

void URoboAimUI::NativeConstruct()
{
    Super::NativeConstruct();

    if (Image_Arrow)
    {
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Arrow->Slot))
        {
            CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
        }
    }
}

void URoboAimUI::ResetAimPos()
{
    if (Image_Arrow)
    {
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Arrow->Slot))
        {
            CanvasSlot->SetPosition(FVector2D::ZeroVector);
        }
    }
}

void URoboAimUI::UpdateAimPos(FVector2D MoveDelta)
{
    if (Image_Arrow)
    {   
        if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Arrow->Slot))
        {         
            FVector2D CurrentPos = CanvasSlot->GetPosition();
            CurrentPos += MoveDelta;

            if (CurrentPos.Length() > ArcRadius)
            {
                CurrentPos = CurrentPos.GetSafeNormal() * ArcRadius;
            }

            CanvasSlot->SetPosition(CurrentPos);
            // 위치 로그 출력
            /*GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Purple, FString::Printf(TEXT("AbsoluteNewPos:%s "), *CurrentPos.ToString()));*/
        }
    }
}

FVector2D URoboAimUI::GetCrosshairScreenPosition() const
{
    if (Image_Arrow)
    {
        const FGeometry& ArrowGeometry = Image_Arrow->GetCachedGeometry();
        return ArrowGeometry.GetAbsolutePosition() + (ArrowGeometry.GetLocalSize() / 2.0f);
    }
    return FVector2D::ZeroVector;
}
