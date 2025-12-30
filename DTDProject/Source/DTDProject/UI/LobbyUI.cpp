// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyUI.h"
#include "UI/ShopUI.h"
#include <Kismet/GameplayStatics.h>
#include "Components/Button.h"
#include "Styling/SlateColor.h"

void ULobbyUI::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼들을 배열에 추가합니다.
    if (Button_NewGame)
    {
        NavigatableButtons.Add(Button_NewGame);
        Button_NewGame->OnClicked.AddDynamic(this, &ULobbyUI::OnNewGameClicked);
    }
    if (Button_ExitGame)
    {
        NavigatableButtons.Add(Button_ExitGame);
        Button_ExitGame->OnClicked.AddDynamic(this, &ULobbyUI::OnExitClicked);
    }

    // 초기 버튼 인덱스 설정
    SelectedButtonIndex = 0;
    UpdateButtonFocus();

    if (ShopWidget)
    {
        ShopWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    // 키 입력을 받도록 설정
    bIsFocusable = true;
}

void ULobbyUI::NavigateUp_Implementation()
{
    SelectedButtonIndex = FMath::Max(0, SelectedButtonIndex - 1);
    UpdateButtonFocus();
}

void ULobbyUI::NavigateDown_Implementation()
{
    SelectedButtonIndex = FMath::Min(NavigatableButtons.Num() - 1, SelectedButtonIndex + 1);
    UpdateButtonFocus();
}

void ULobbyUI::Select_Implementation()
{
    if (NavigatableButtons.IsValidIndex(SelectedButtonIndex))
    {
        // 현재 선택된 버튼의 클릭 이벤트를 실행합니다.
        NavigatableButtons[SelectedButtonIndex]->OnClicked.Broadcast();
    }
}


void ULobbyUI::UpdateButtonFocus()
{
    for (int32 i = 0; i < NavigatableButtons.Num(); ++i)
    {
        if (i == SelectedButtonIndex)
        {
            // 선택된 버튼: 밝은 색으로 강조
            NavigatableButtons[i]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
        }
        else
        {
            // 선택되지 않은 버튼: 약간 어두운 색으로
            NavigatableButtons[i]->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
        }
    }
}

void ULobbyUI::OnNewGameClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("New Game Button Clicked!"));
	UGameplayStatics::OpenLevel(this, "Map1Level");
}

void ULobbyUI::OnExitClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Exit Game Button Clicked!"));
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void ULobbyUI::ShowShop_Implementation()
{
    if (ShopWidget)
    {
        ShopWidget->SetVisibility(ESlateVisibility::Visible);
        UE_LOG(LogTemp, Warning, TEXT("Shop shown from Lobby UI"));
    }
}
