// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultTableUI.generated.h"

/**
 * 산소 핵심 + 랜덤상자 설계(기본)
무기별 차이 정리 + 연출 강화
Fast-Exit(인양기) 기본(옵션) + 마무리 루프 + 포트폴리오 정리

산소 30이면
화면 빨갛게 경고

산소 0이면
획득한 물고기 UI 띄우기 / (물고기 중 하나만 고르기) / 게임 종료 / 첫화면으로 돌아가기  
 */
UCLASS()
class DTDPROJECT_API UResultTableUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Table;
};
