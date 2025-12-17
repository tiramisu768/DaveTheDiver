// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ResponsiveUI.h"
#include "ResultTableUI.generated.h"

UCLASS()
class DTDPROJECT_API UResultTableUI : public UResponsiveUI
{
	GENERATED_BODY()

public:
	void SetGameEnd(bool bWasSuccessful);

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
};
