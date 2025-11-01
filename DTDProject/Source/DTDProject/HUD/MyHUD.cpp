// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/RoboHPBarUI.h"
#include "UI/RoboAimUI.h"
#include "UI/RoboWeaponUI.h"
#include "UI/ResultTableUI.h"
#include "UI/WarningOxygenUI.h"
#include "UI/FishRankUI.h"
#include "Engine/Canvas.h"

void AMyHUD::ShowLobbyUI()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}

	if (LobbyWidget)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), LobbyWidget);
		CurrentWidget->AddToViewport();
	}
}

void AMyHUD::ShowMainUI()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}

	if (MainWidget)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidget);
		CurrentWidget->AddToViewport();
	}
}

void AMyHUD::OnEnterGame()
{
	ShowMainUI();

}

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_LobbyUI.BP_LobbyUI_C"));
	if (LobbyWidgetClassFinder.Succeeded())
	{
		LobbyWidget = LobbyWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_RoboHPBar.BP_RoboHPBar_C"));
	if (MainWidgetClassFinder.Succeeded())
	{
		MainWidget = MainWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_RoboHPBar.BP_RoboHPBar_C"));
	if (HPBarWidgetClassFinder.Succeeded())
	{
		HPBarWidget = HPBarWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WarningOxygenWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_WarningOxygen.BP_WarningOxygen_C"));
	if (WarningOxygenWidgetClassFinder.Succeeded())
	{
		WarningOxygenWidget = WarningOxygenWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WeaponWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_WeaponUI.BP_WeaponUI_C"));
	if (WeaponWidgetClassFinder.Succeeded())
	{
		WeaponWidget = WeaponWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> RankWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Rank.BP_Rank_C"));
	if (RankWidgetClassFinder.Succeeded())
	{
		RankWidget = RankWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> AimWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Aim.BP_Aim_C"));
	if (AimWidgetClassFinder.Succeeded())
	{
		AimWidget = AimWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ResultTableWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_ResultTable.BP_ResultTable_C"));
	if (ResultTableWidgetClassFinder.Succeeded())
	{
		ResultTableWidget = ResultTableWidgetClassFinder.Class;
	}
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	// 레벨 이름 로그 출력
	FString LevelName = GetWorld()->GetMapName();
	if (LevelName == TEXT("UEDPIE_0_LobbyLevel"))
		ShowLobbyUI();
	else
		ShowMainUI();

	/*if (HPBarWidget)
	{
		RoboHPBarUIClass = CreateWidget<URoboHPBarUI>(GetWorld(), HPBarWidget);
		if (RoboHPBarUIClass)
		{
			RoboHPBarUIClass->AddToViewport();
		}
	}

	if (WarningOxygenWidget)
	{
		WarningOxygenUIClass = CreateWidget<UWarningOxygenUI>(GetWorld(), WarningOxygenWidget);
		if (WarningOxygenUIClass)
		{
			WarningOxygenUIClass->AddToViewport();
			WarningOxygenUIClass->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WeaponWidget)
	{
		RoboWeaponUIClass = CreateWidget<URoboWeaponUI>(GetWorld(), WeaponWidget);
		if (RoboWeaponUIClass)
		{
			RoboWeaponUIClass->AddToViewport();
		}
	}

	if (RankWidget)
	{
		RankUIClass = CreateWidget<UFishRankUI>(GetWorld(), RankWidget);
		if (RankUIClass)
		{
			RankUIClass->AddToViewport();
		}
	}

	if (AimWidget)
	{
		RoboAimUIClass = CreateWidget<URoboAimUI>(GetWorld(), AimWidget);
		if (RoboAimUIClass)
		{
			RoboAimUIClass->AddToViewport();
			RoboAimUIClass->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (ResultTableWidget)
	{
		ResultTableUIClass = CreateWidget<UResultTableUI>(GetWorld(), ResultTableWidget);
		if (ResultTableUIClass)
		{
			ResultTableUIClass->AddToViewport();
			ResultTableUIClass->SetVisibility(ESlateVisibility::Hidden);
		}
	}*/
}


void AMyHUD::SetHPPercent(float value)
{
	if(RoboHPBarUIClass)
	{
		RoboHPBarUIClass->SetHPBarPercent(value);
	}
}

void AMyHUD::SetMeters(float value)
{
	if (RoboHPBarUIClass)
	{
		RoboHPBarUIClass->SetCurrentDepthMeters(value);
	}
}

void AMyHUD::SetWeights(float Current, float Max)
{
	if (RoboHPBarUIClass)
	{
		RoboHPBarUIClass->SetCurrentAndMaxWeight(Current, Max);
	}
}

void AMyHUD::ShowRankUI(const TArray<FCaughtFishInfo>& FishList)
{
	if(RankUIClass)
	{
		RankUIClass->UpdateFishRankList(FishList);
		RankUIClass->ShowRank();
	}
}

void AMyHUD::ShowOxygenWarningUI()
{
	if (WarningOxygenUIClass)
	{
		WarningOxygenUIClass->SetVisibility(ESlateVisibility::Visible);
		WarningOxygenUIClass->SetOxygenWarning();
	}
}

void AMyHUD::PlaySwitchAnimation(int32 SelectedIndex)
{
	if (RoboWeaponUIClass)
	{
		RoboWeaponUIClass->PlaySwitchAnimation(SelectedIndex);
	}
}

void AMyHUD::UpdateAimPos(FVector2D AimPos)
{
	if (RoboAimUIClass)
	{
		RoboAimUIClass->UpdateAimPos(AimPos);
	}
}

void AMyHUD::ResetAimPos()
{
	if (RoboAimUIClass)
	{
		RoboAimUIClass->ResetAimPos();
	}
}

void AMyHUD::ShowGameEndUI()
{
	if (ResultTableUIClass)
	{
		ResultTableUIClass->SetVisibility(ESlateVisibility::Visible);
		ResultTableUIClass->SetGameEnd();
	}
}
