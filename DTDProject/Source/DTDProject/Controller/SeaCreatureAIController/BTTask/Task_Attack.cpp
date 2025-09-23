// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Attack.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "MyRobo/MyRobo.h"

UTask_Attack::UTask_Attack()
{
}

EBTNodeResult::Type UTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ASeaCreature* Owner = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr) return EBTNodeResult::Failed;

	AMyRobo* Target = Cast<AMyRobo>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (Target == nullptr) return EBTNodeResult::Failed;

	Owner->Attack(Target);

	return EBTNodeResult::InProgress;
}
