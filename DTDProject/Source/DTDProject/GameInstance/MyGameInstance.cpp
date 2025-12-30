// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MyGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UMyGameInstance::StartNewGameRandom()
{
	const TArray<FName> MapNames = { FName("Map1Level"), FName("Map2Level") };
	if (MapNames.Num() == 0) return;

	const int32 Index = FMath::RandRange(0, MapNames.Num() - 1);
	UGameplayStatics::OpenLevel(GetWorld(), MapNames[Index]);
}
