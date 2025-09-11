// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Decorator_WanderRadius.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UDecorator_WanderRadius : public UBTDecorator
{
	GENERATED_BODY()
public:
	UDecorator_WanderRadius();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
