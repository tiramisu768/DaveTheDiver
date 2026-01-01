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

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<class UButton> ConfirmButton;

private:
	UFUNCTION()
	void OnConfirmClicked();

	FTimerHandle HideAnimFinishTimerHandle;

	void BroadcastShopClosed();
};
