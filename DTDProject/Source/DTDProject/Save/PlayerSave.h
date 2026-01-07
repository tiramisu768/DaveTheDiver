// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ActorComponent/InventoryComponent.h"
#include "PlayerSave.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UPlayerSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 Coins = 0;

	UPROPERTY()
	TArray<FCaughtFishInfo> TempCaughtFishList;

	UPROPERTY()
	FCaughtFishInfo SelectedFish;

	UPROPERTY()
	bool bHasSelectedFish = false;

	UPROPERTY()
	FName NextMapName;
};
