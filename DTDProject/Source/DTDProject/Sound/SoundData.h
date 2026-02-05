// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "SoundData.generated.h"

UENUM(BlueprintType)
enum class ESoundKey :uint8
{
	None,
	UI_Confirm,
	UI_Switch,
	Interaction_Start,
	Interaction_Pickup,
	Player_Aim,
	Player_Hurt,
	Player_Collect,
	Player_Swim,
	Weapon_Melee,
	Weapon_Bat,
	Weapon_Pistol,
	Weapon_Shoot,
	BGM_Lobby,
	BGM_InGame
};

USTRUCT(BlueprintType)
struct FSoundData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DefaultVolume = 1.0f;
	
};
