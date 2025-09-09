// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Wander.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"

void UTask_Wander::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	ASeaCreature* SeaCreature = nullptr;

	if (nullptr != Owner)
		SeaCreature = Cast<ASeaCreature>(Owner->GetPawn());
	check(SeaCreature);
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject("MoveTarget"));

	if (nullptr != Target)
	{
		//task다음에 selector일 때 failed, sequence안에 있으면 succeeded로 해야함
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector dir = SeaCreature->SteeringComp->ComputeWanderDir(DeltaSeconds);
	dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	SeaCreature->SteeringComp->ComputeApplyMoveInput(dir.GetSafeNormal());
}
