// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MyCharacterController/LoadingController.h"
#include "Kismet/GameplayStatics.h"
#include "Save/PlayerSave.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ALoadingController::ALoadingController()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALoadingController::BeginPlay()
{
	Super::BeginPlay();

	//save 불러오기
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("no save slot"));
		return;
	}

	USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0);
	UPlayerSave* Save = Cast<UPlayerSave>(Loaded);
	if (!Save)
	{
		UE_LOG(LogTemp, Warning, TEXT("fail Load"));
		return;
	}

	NextMapToLoad = Save->NextMapName;
	if (NextMapToLoad.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingController::BeginPlay - NextMap is None. Aborting OpenLevel. Check Save->NextMapName before switching to LoadingLevel."));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Loading next map:%s"),*NextMapToLoad.ToString());

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadingController::BeginPlay - World is null"));
		return;
	}

	if (LoadingWidgetClass)
	{
		LoadingWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
		if (LoadingWidgetInstance)
		{
			LoadingWidgetInstance->AddToViewport(1000);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("loadingwidget null"));
		}
	}

	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		Controller->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		Controller->SetInputMode(InputMode);

		APawn* Pawn = Controller->GetPawn();
		if (Pawn)
		{
			Pawn->DisableInput(Controller);
			Pawn->SetActorHiddenInGame(true);
			Pawn->SetActorEnableCollision(false);
		}
	}

	FTimerHandle TH;
	World->GetTimerManager().SetTimer(TH, this, &ALoadingController::LoadNextMap, 5.0f, false);
}

void ALoadingController::LoadNextMap()
{
	if (LoadingWidgetInstance)
	{
		LoadingWidgetInstance->RemoveFromParent();
		LoadingWidgetInstance = nullptr;
	}

	if (NextMapToLoad.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("NextMapToLoad is None"));
		return;
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadNextMap - World is null. Cannot open level."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("LoadNextMap - Now opening level: %s"), *NextMapToLoad.ToString());
	UGameplayStatics::OpenLevel(GetWorld(), NextMapToLoad);
}

// Called every frame
void ALoadingController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

