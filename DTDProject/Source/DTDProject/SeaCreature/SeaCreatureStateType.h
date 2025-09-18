// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SeaCreatureStateType.generated.h"

UENUM(BlueprintType)
enum class ESeaCreatureState : uint8
{
    Wander,
    Flee,
    Seek,
    Attack,
    ReturnHome
};