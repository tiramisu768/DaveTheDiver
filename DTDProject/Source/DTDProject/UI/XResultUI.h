// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XResultUI.generated.h"

class UVerticalBox;

UCLASS()
class DTDPROJECT_API UXResultUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void PopulateFishList(const TArray<struct FCaughtFishInfo>& FishList);

	DECLARE_DELEGATE_OneParam(FOnFishSelected,int32)
	FOnFishSelected OnFishSelectedDelegate;

protected:
	//virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "Result Table", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> XResultItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> XResultVerticalBox;

private:
	UFUNCTION()
	void HandleFishSelection(int32 Index);
};
