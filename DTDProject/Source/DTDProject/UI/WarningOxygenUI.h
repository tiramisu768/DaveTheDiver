// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarningOxygenUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UWarningOxygenUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Table;

public:
	void SetOxygenWarning();
};
