// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTDecorator/Decorator_WanderRadius.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"

UDecorator_WanderRadius::UDecorator_WanderRadius()
{
	NodeName = TEXT("CanWander (DistanceFromHome <= Wander Radius)");
}

bool UDecorator_WanderRadius::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(Owner->GetPawn());
	
	if (!Owner||!BlackboardComp || !SeaCreature) return false;

	const float DistFromHome = BlackboardComp->GetValueAsFloat(TEXT("DistanceFromHome"));
	const float WanderRadius = SeaCreature->GetData()->WanderRadius;

	return DistFromHome <= WanderRadius;
}
