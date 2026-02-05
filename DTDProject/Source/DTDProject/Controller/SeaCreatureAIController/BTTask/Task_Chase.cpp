// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Chase.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ActorComponent/StateComponent/StateComponent.h"

UTask_Chase::UTask_Chase()
{
	bNotifyTick = true;
	NodeName = TEXT("Chase");
}

EBTNodeResult::Type UTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (SeaCreature == nullptr || SeaCreature->GetData() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	SeaCreature->StartEffect();

	SeaCreature->MovementComponent->MaxSpeed = SeaCreature->GetData()->ChaseSpeed;
	SeaCreature->MovementComponent->Acceleration = SeaCreature->GetData()->Acceleration;
	
	return EBTNodeResult::InProgress;
}

void UTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());

	if (BlackboardComp==nullptr || SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));

	//타겟이 사라졌거나 너무 멀어지면 추격 포기 (Succeeded)
	const float DistToTarget = FVector::Dist(SeaCreature->GetActorLocation(), TargetActor->GetActorLocation());
	bool bChaseFinished = false;
	if (TargetActor == nullptr)
	{
		bChaseFinished = true;
	}
	else
	{
		if (DistToTarget > SeaCreature->GetData()->SafeDistance)
		{
			bChaseFinished = true;
		}
	}

	if (bChaseFinished)
	{
		BlackboardComp->ClearValue(ASeaCreatureAIController::TargetActorKey);
		BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsThreatImminentKey, false);
		BlackboardComp->SetValueAsBool(ASeaCreatureAIController::ChaseTargetLocationKey, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	//타겟이 공격 범위 안에 들어오면 추격 성공 (Succeeded)
	if (DistToTarget <= SeaCreature->GetData()->AttackRange)
	{
		// 움직임을 멈추고 성공을 반환합니다.
		//SeaCreature->MovementComponent->StopMovementImmediately();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}


	FVector Dir = SeaCreature->SteeringComp->ComputeChaseDir(TargetActor->GetActorLocation());
	Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	Dir.Normalize();

	if (!Dir.IsNearlyZero())
	{
		SeaCreature->AddMovementInput(Dir);
		FRotator TargetRotation = Dir.Rotation();
		SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
	}
}

void UTask_Chase::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		ASeaCreature* SeaCreature = Cast<ASeaCreature>(AIController->GetPawn());
		if (SeaCreature)
		{
			SeaCreature->StopEffect();
		}
	}
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
