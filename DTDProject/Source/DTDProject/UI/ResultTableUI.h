// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ResponsiveUI.h"
#include "ResultTableUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResultConfirmed, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResultConfirmRequested, bool, bSuccess, int32, SelectedIndex);

UCLASS()
class DTDPROJECT_API UResultTableUI : public UResponsiveUI
{
	GENERATED_BODY()

public:
	void SetGameEnd(bool bWasSuccessful);

	UPROPERTY(BlueprintAssignable, Category = "Result")
	FResultConfirmed OnResultConfirmed;

	UPROPERTY(BlueprintAssignable, Category = "Result")
	FResultConfirmRequested OnConfirmRequested;

	UFUNCTION(BlueprintImplementableEvent, Category="Result")
	void PopulateFishList(const TArray<struct FCaughtFishInfo>& FishList);

	UFUNCTION(BlueprintCallable, Category="Result")
	void SelectFishAtIndex(int32 Index);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> ResultSwitcher;

	const int32 GoodResultIndex = 0;
	const int32 BadResultIndex = 1;

	UFUNCTION()
	void OnConfirmClicked();

	int32 SelectedFishIndex = -1; //실패화면에서 선택된 물고기 인덱스

	void UpdateConfirmButtonState();

	void BroadcastResultConfirmed();

	bool bPendingSuccess = false;
	FTimerHandle HideAnimFinishTimerHandle;
};
