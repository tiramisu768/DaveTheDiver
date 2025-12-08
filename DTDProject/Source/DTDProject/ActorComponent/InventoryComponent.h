// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	// 필요시 기능적 데이터 참조
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FName DataTableRowName;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFishCollectedSignature, const FCaughtFishInfo&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedSignature, const TArray<FCaughtFishInfo>&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTDPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	FOnFishCollectedSignature OnFishCollected;
	FOnInventoryChangedSignature OnInventoryChanged;

	void AddCaughtFish(const FCaughtFishInfo& Info);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<FCaughtFishInfo> CaughtFishList;
};
