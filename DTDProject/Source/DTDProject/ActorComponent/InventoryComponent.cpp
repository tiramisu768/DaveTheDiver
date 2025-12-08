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
	OnFishCollected.ExecuteIfBound(Info);
}

