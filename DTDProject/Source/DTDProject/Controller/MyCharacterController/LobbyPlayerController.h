// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();
	
protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	UPROPERTY()
	class UUserWidget* LobbyWidgetInstance;
};
