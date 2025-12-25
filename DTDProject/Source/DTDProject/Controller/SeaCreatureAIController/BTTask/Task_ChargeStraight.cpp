// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_ChargeStraight.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "GameFramework/FloatingPawnMovement.h"

UTask_ChargeStraight::UTask_ChargeStraight()
{
	bNotifyTick = true;
	NodeName = TEXT("Charge Straight");
}

EBTNodeResult::Type UTask_ChargeStraight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (SeaCreature == nullptr || SeaCreature->GetData() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	SeaCreature->MovementComponent->MaxSpeed = SeaCreature->GetData()->ChaseSpeed;
	SeaCreature->MovementComponent->Acceleration = SeaCreature->GetData()->Acceleration;

	FVector Goal = BlackboardComp->GetValueAsVector(TEXT("LastKnowTargetLocation"));
	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
	if (Goal.IsNearlyZero() && Target)
	{
		Goal = Target->GetActorLocation();
		BlackboardComp->SetValueAsVector(TEXT("LastKnowTargetLocation"), Goal);
	}

	if (Goal.IsNearlyZero()) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UTask_ChargeStraight::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());

	if (BlackboardComp == nullptr || SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector Goal = BlackboardComp->GetValueAsVector(TEXT("LastKnowTargetLocation"));
	if (Goal.IsNearlyZero())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//타겟이 사라졌거나 너무 멀어지면 추격 포기 (Succeeded)
	FVector Pos = SeaCreature->GetActorLocation();
	const float DistToTarget = FVector::Dist2D(Pos, Goal);

	if (DistToTarget > SeaCreature->GetData()->SafeDistance)
	{
		BlackboardComp->ClearValue(ASeaCreatureAIController::TargetActorKey);
		BlackboardComp->ClearValue(TEXT("LastKnowTargetLocation"));
		BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsChargingKey, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed); //succeed??
		return;
	}

	//타겟이 공격 범위 안에 들어오면 추격 성공 (Succeeded)
	if (DistToTarget <= SeaCreature->GetData()->AttackRange)
	{
		// 돌격이 끝났으니 충전 플래그 클리어 (Attack Task가 다시 클리어할 수도 있음)
		BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsChargingKey, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}


	FVector Dir = Goal-Pos;
	Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	Dir.Z = 0.f;
	Dir.Normalize();

	if (!Dir.IsNearlyZero())
	{
		SeaCreature->AddMovementInput(Dir, SeaCreature->GetData()->ChaseSpeed);
		FRotator TargetRotation = Dir.Rotation();
		SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
	}
}
