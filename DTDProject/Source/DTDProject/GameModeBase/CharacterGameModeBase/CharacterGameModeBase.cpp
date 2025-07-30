// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/CharacterGameModeBase/CharacterGameModeBase.h"
#include "CharacterGameModeBase.h"
#include "MyCharacter/MyCharacter.h"
#include "Controller/MyCharacterController/MyCharacterController.h"

ACharacterGameModeBase::ACharacterGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrint/MyCharacter/BP_MyCharacter.BP_MyCharacter_C"));
	if (PlayerPawnBPClass.Succeeded())
		DefaultPawnClass = PlayerPawnBPClass.Class;

	PlayerControllerClass = AMyCharacterController::StaticClass();
}
