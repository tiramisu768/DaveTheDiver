// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Seek.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "SeaCreature/SeaCreatureStateType.h"

void UTask_Seek::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	ASeaCreature* SeaCreature = nullptr;

	if (nullptr != Owner)
		SeaCreature = Cast<ASeaCreature>(Owner->GetPawn());

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));

	if ( nullptr == Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//데이터테이블의 speed를 가져오기
	float Speed=0.f;
	if (const FSeaCreatureData* Stats = SeaCreature->GetData())
		Speed = Stats->SeekSpeed;

	FVector dir = SeaCreature->SteeringComp->ComputeSeekDir(Target->GetActorLocation());
	dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();

	SeaCreature->SteeringComp->ComputeApplyMoveInput(dir.GetSafeNormal(), Speed);
}
