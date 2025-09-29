// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTDecorator/Decorator_WanderRadius.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UDecorator_WanderRadius::UDecorator_WanderRadius()
{
	NodeName = TEXT("Wander Radius (DistanceFromHome <= HomeReturnDist)");
}

bool UDecorator_WanderRadius::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	const float DistFromHome = BlackboardComp->GetValueAsFloat(TEXT("DistanceFromHome"));
	const float HomeReturnDist = BlackboardComp->GetValueAsFloat(TEXT("HomeReturnDist"));

	return DistFromHome <= HomeReturnDist;
}
