// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/UINavigateInterface.h" // 인터페이스 헤더 추가
#include "LobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API ULobbyUI : public UUserWidget, public IUINavigateInterface // 인터페이스 상속
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	// virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override; // 더 이상 필요 없음

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Button_NewGame;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Button_ExitGame;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UButton>> NavigatableButtons;

	int32 SelectedButtonIndex = 0;

	void UpdateButtonFocus();

	// UINavigateInterface 구현
	virtual void NavigateUp_Implementation() override;
	virtual void NavigateDown_Implementation() override;
	virtual void Select_Implementation() override;


public:
	void OnNewGameClicked();
	void OnExitClicked();
	
};
