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

DECLARE_DELEGATE_OneParam(FOnInventoryChanged, const TArray<FCaughtFishInfo>&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTDPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FCaughtFishInfo> CaughtFishList;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnInventoryChanged OnInventoryChanged;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AddCaughtFish(const FCaughtFishInfo& Info);
};
