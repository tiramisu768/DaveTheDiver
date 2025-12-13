// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeBase/CharacterGameModeBase/MyGameState.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::SellAllFish()
{
	int32 TotalPrice = 0;
	for (const FCaughtFishInfo& Fish : CaughtFishList)
	{
		TotalPrice += Fish.Grade * 10 + static_cast<int32>(Fish.Weight * 5);
	}

	if (AMyGameState* MyGameState = GetWorld()->GetGameState<AMyGameState>())
	{
		//MyGameState->AddCoins(TotalPrice);
	}

	CaughtFishList.Empty();
	CurrentTotalWeight = 0.0f;
	//IsOverweight = false;

	OnWeightChanged.Broadcast(CurrentTotalWeight, 30.f);
	OnBecameOverweight.Broadcast(false);
}

void UInventoryComponent::SwitchActiveTool()
{
	ActiveToolSlotIndex = (ActiveToolSlotIndex + 1) % 2;
	OnActiveToolChanged.Broadcast(ActiveToolSlotIndex);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::AddCaughtFish(const FCaughtFishInfo& Info)
{
	CaughtFishList.Add(Info);

	CurrentTotalWeight += Info.Weight;

	OnFishCollected.Broadcast(Info);

	float MaxWeight = 30.f;
	OnWeightChanged.Broadcast(CurrentTotalWeight, MaxWeight);

	if(CurrentTotalWeight >= MaxWeight)
	{
		OnBecameOverweight.Broadcast(true);
	}
}

