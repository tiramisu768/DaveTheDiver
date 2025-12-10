// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

