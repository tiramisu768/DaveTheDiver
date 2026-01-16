// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ResponsiveUI.h"
#include "ShopUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShopClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShopConfirmRequested);

UCLASS()
class DTDPROJECT_API UShopUI : public UResponsiveUI
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category="Shop")
	FShopConfirmRequested OnConfirmRequested;

	UPROPERTY(BlueprintAssignable, Category="Shop")
	FShopClosed OnShopClosed;

	void UpdateCoinCount(int32 NewCoinAmount);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ConfirmButton;

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<class UTextBlock> CoinText;

private:
	UFUNCTION()
	void OnConfirmClicked();

	FTimerHandle HideAnimFinishTimerHandle;

	UFUNCTION()
	void BroadcastShopClosed();
};
