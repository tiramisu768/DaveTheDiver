// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_PerceptionUpdate.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UService_PerceptionUpdate : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UService_PerceptionUpdate();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
