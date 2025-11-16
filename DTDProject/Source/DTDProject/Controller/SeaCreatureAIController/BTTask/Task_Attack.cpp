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
	// 이 태스크는 한 번만 실행되고 몽타주가 끝날 때까지 기다리므로, Tick이 필요 없음
	bNotifyTick = false;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (SeaCreature == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AMyRobo* Target = Cast<AMyRobo>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	SeaCreature->OnAttackMontageEndedDelegate.BindLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	SeaCreature->Attack(Target);

	return EBTNodeResult::InProgress;
}
