// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Object/PickupItem.h"
#include "GameModeBase/CharacterGameModeBase/MyGameState.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ToolSlots.SetNum(MaxToolSlots);
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

bool UInventoryComponent::IsToolInventoryFull() const
{
	return ToolSlots.Num()>=MaxToolSlots;
}

void UInventoryComponent::SetToolInSlot(int32 SlotIndex, APickupItem* Tool)
{
	if (ToolSlots.IsValidIndex(SlotIndex))
	{
		ToolSlots[SlotIndex] = Tool;
		// 툴이 유효하면 RowName을, 아니면 FName()을 전달
		/*const FName ToolRowName = Tool ? Tool->RowName : FName();
		OnToolSlotUpdated.Broadcast(SlotIndex, ToolRowName);*/
	}
}

void UInventoryComponent::AddTool(APickupItem* NewTool)
{
	if (!IsToolInventoryFull())
	{
		int32 NewIndex = ToolSlots.Add(NewTool);
		//OnToolSlotUpdated.Broadcast(NewIndex, NewTool);
	}
}

APickupItem* UInventoryComponent::RemoveToolFromSlot(int32 SlotIndex)
{
	if (ToolSlots.IsValidIndex(SlotIndex))
	{
		APickupItem* RemovedTool = ToolSlots[SlotIndex];
		ToolSlots[SlotIndex]= nullptr;
		OnToolSlotUpdated.Broadcast(SlotIndex,FName());
		return RemovedTool;
	}
	return nullptr;
}

APickupItem* UInventoryComponent::GetToolFromSlot(int32 SlotIndex) const
{
	if (ToolSlots.IsValidIndex(SlotIndex))
	{
		return ToolSlots[SlotIndex];
	}
	return nullptr;
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

