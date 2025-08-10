// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/CharacterGameModeBase/CharacterGameModeBase.h"
#include "CharacterGameModeBase.h"
#include  "MyRobo/MyRobo.h"
#include "Controller/MyCharacterController/MyCharacterController.h"
#include "HUD/MyHUD.h"

ACharacterGameModeBase::ACharacterGameModeBase()
{
	static ConstructorHelpers::FClassFinder<AMyRobo> MyRoboBPClass(TEXT("/Game/BluePrint/MyRobo/BP_MyRobo.BP_MyRobo_C"));
	if (MyRoboBPClass.Succeeded())
		DefaultPawnClass = MyRoboBPClass.Class;

	PlayerControllerClass = AMyCharacterController::StaticClass();
	HUDClass = AMyHUD::StaticClass();
}
