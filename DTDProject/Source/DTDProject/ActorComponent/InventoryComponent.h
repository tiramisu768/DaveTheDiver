// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tool/ToolData.h"
#include "Object/PickupItem.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FCaughtFishInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FishName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Grade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StarCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> FishIcon;

	// 필요시 기능적 데이터 참조
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FName DataTableRowName;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFishCollected, const FCaughtFishInfo&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeightChanged, float, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBecameOverweight, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToolSlotUpdated, int32, FName);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnActiveToolChanged,int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<FCaughtFishInfo>&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTDPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	FOnFishCollected OnFishCollected;
	FOnWeightChanged OnWeightChanged;
	FOnBecameOverweight OnBecameOverweight;
	FOnToolSlotUpdated OnToolSlotUpdated;
	FOnActiveToolChanged OnActiveToolChanged;
	FOnInventoryChanged OnInventoryChanged;

	void AddCaughtFish(const FCaughtFishInfo& Info);

	const TArray<FCaughtFishInfo>& GetCaughtFishList() const { return CaughtFishList; }

	void SellAllFish();
	void SwitchActiveTool();
	bool IsToolInventoryFull() const;
	void SetToolInSlot(int32 SlotIndex, APickupItem* Tool);
	void AddTool(APickupItem* NewTool);
	APickupItem* RemoveToolFromSlot(int32 SlotIndex);
	APickupItem* GetToolFromSlot(int32 SlotIndex) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	float CurrentTotalWeight;

	UPROPERTY()
	TArray<FCaughtFishInfo> CaughtFishList;

	UPROPERTY(EditDefaultsOnly,Category ="Tool")
	TObjectPtr<UDataTable> ToolDataTable;

	int32 ActiveToolSlotIndex;

	UPROPERTY()
	TArray<TObjectPtr<APickupItem>> ToolSlots;

	UPROPERTY(EditDefaultsOnly,Category = "Inventory")
	int32 MaxToolSlots = 2;
};
