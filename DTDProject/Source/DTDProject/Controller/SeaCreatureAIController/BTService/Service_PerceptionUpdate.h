// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_PerceptionUpdate.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UService_PerceptionUpdate : public UBTService
{
	GENERATED_BODY()

public:
	UService_PerceptionUpdate();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
