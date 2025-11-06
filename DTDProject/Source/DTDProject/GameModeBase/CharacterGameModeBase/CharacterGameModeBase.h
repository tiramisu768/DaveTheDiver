// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API ACharacterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACharacterGameModeBase();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "GameModeBase")
	float LimitZ = .0f;

};
