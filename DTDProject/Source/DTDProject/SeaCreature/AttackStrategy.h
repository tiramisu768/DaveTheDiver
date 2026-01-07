// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttackStrategy.generated.h"

class ASeaCreature;
class AMyRobo;

/**
 * 공격 행동을 정의하는 기본 전략 클래스입니다.
 * 각 물고기 타입은 이 클래스를 상속받아 자신만의 공격 방식을 구현합니다.
 */
UCLASS(Abstract, Blueprintable)
class DTDPROJECT_API UAttackStrategy : public UObject
{
	GENERATED_BODY()
public:
	bool CanExecuteAttack(ASeaCreature* Attacker, AMyRobo* Target);
	virtual void ExecuteAttack(ASeaCreature* Attacker, AMyRobo* Target);
};
