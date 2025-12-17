// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ActorComponent/InventoryComponent.h"
#include "MyGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnGameEnded);

UCLASS()
class DTDPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FOnGameEnded OnGameEnded;

	void AddCoins(int32 Amount) { Coins += Amount; }
	int32 GetCoins() const { return Coins; }

	void SetTempCaughtFishList(const TArray<FCaughtFishInfo>& List) { TempCaughtFishList = List; }
	const TArray<FCaughtFishInfo>& GetTempCaughtFishList() const { return TempCaughtFishList; }
	void ClearTempCaughtFishList() { TempCaughtFishList.Empty(); }

	void SetSelectedFish(const FCaughtFishInfo& Fish) { SelectedFish = Fish; bHasSelectedFish = true; }
	bool HasSelectedFish() const { return bHasSelectedFish; }
	const FCaughtFishInfo& GetSelectedFish() const { return SelectedFish; }

private:
	UPROPERTY()
	int32 Coins = 0;

	UPROPERTY()
	TArray<FCaughtFishInfo> TempCaughtFishList;

	UPROPERTY()
	FCaughtFishInfo SelectedFish;

	UPROPERTY()
	bool bHasSelectedFish = false;
	
};
