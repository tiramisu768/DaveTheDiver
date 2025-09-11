// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTDecorator/Decorator_WanderRadius.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UDecorator_WanderRadius::UDecorator_WanderRadius()
{
	NodeName = TEXT("Wander Radius");
}

bool UDecorator_WanderRadius::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerPawn == nullptr) return false;
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetObject")));
	if (TargetPawn == nullptr) return false;
	float Distance = OwnerPawn->GetDistanceTo(TargetPawn);
	//Distance = FVector::Dist(OwnerPawn->GetActorLocation(), TargetPawn->GetActorLocation());
	if (Distance <= 300.f) return true;

	return false;
}
