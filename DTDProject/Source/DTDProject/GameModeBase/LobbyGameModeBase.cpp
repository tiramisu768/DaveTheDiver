// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/LobbyGameModeBase.h"
#include "Controller/MyCharacterController/LobbyPlayerController.h"

ALobbyGameModeBase::ALobbyGameModeBase()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();

	HUDClass = nullptr;
}
