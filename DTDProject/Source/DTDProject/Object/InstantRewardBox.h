// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/BaseBox.h"
#include "InstantRewardBox.generated.h"

UENUM(BlueprintType)
enum class EInstantRewardType :uint8
{
	Ammo UMETA(DisplayName = "Ammo"),
	Oxygen UMETA(DisplayName = "Oxygen"),
};

UCLASS()
class DTDPROJECT_API AInstantRewardBox : public ABaseBox
{
	GENERATED_BODY()

protected:
	virtual void OnBoxOpened() override;

	UPROPERTY(EditAnywhere, Category = "Reward")
	EInstantRewardType RewardType = EInstantRewardType::Oxygen;

	UPROPERTY(EditAnywhere, Category = "Reward|Oxygen")
	float OxygenRefillAmount = 50.f;
};
