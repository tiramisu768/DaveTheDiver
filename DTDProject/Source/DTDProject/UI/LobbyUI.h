// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/UINavigateInterface.h"
#include "LobbyUI.generated.h"

class UShopUI;

UCLASS()
class DTDPROJECT_API ULobbyUI : public UUserWidget, public IUINavigateInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Button_NewGame;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Button_ExitGame;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UButton>> NavigatableButtons;

	int32 SelectedButtonIndex = 0;

	void UpdateButtonFocus();

	// UINavigateInterface ±¸Çö
	virtual void NavigateUp_Implementation() override;
	virtual void NavigateDown_Implementation() override;
	virtual void Select_Implementation() override;

public:
	void OnNewGameClicked();
	void OnExitClicked();
	
};
