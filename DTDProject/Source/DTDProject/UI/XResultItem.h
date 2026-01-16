// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorComponent/InventoryComponent.h"
#include "XResultItem.generated.h"

class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelected, int32, ItemIndex);

UCLASS()
class DTDPROJECT_API UXResultItem : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetItemInfo(const FCaughtFishInfo& Info, int32 Index);

	UPROPERTY(BlueprintAssignable, Category="Item")
	FOnItemSelected OnItemSelected;

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> FishIconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FishName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FishGrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FishWeight;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	int32 ItemIndex = -1;

	UFUNCTION(BlueprintCallable, Category="Item")
	void NotifyItemSelected();
};
