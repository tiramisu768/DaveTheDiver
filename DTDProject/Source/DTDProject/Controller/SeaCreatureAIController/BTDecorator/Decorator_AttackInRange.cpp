// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTDecorator/Decorator_AttackInRange.h"
#include "Decorator_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UDecorator_AttackInRange::UDecorator_AttackInRange()
{
	NodeName = TEXT("Attack In Range");
}

bool UDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerPawn == nullptr) return false;

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (TargetPawn == nullptr) return false;

	float Dist = OwnerPawn->GetDistanceTo(TargetPawn);
	if (Dist <= 300.f) return true;

	return false;
}
