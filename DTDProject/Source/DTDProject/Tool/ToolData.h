// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ToolData.generated.h"

UENUM(BlueprintType)
enum class EToolType : uint8
{
	None,
	Consumable, //일회성 소모품
	Active,     //일정시간 지속되는 아이템
	Passive     //장착으로 효과보는 패시브 아이템
};

USTRUCT(BlueprintType)
struct FToolData :public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="1. Default Info")
	FString ToolName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Default Info")
	TSoftObjectPtr<UTexture2D> ToolIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Type and Effect")
	EToolType ToolType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Type and Effect", meta = (EditCondition="Consumable"))
	float HealAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Type and Effect", meta=(EditCondition="Active"))
	float Duration; //지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Type and Effect", meta = (EditCondition = "Active"))
	float SpeedMultiplier; //속도 증가 배율


};

