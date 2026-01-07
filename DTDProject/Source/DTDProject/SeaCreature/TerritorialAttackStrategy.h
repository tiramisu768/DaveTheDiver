// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SeaCreature/AttackStrategy.h"
#include "TerritorialAttackStrategy.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UTerritorialAttackStrategy : public UAttackStrategy
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteAttack(ASeaCreature* Attacker, AMyRobo* Target) override;
};
