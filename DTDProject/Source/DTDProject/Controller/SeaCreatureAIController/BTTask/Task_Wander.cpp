// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Wander.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

UTask_Wander::UTask_Wander()
{
	bNotifyTick = true;
	NodeName = TEXT("Wander");
}

EBTNodeResult::Type UTask_Wander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	const FSeaCreatureData* FishData = SeaCreature->GetData();
	SeaCreature->MovementComponent->MaxSpeed = FishData->WanderSpeed;
	SeaCreature->MovementComponent->Acceleration = FishData->Acceleration;
	return EBTNodeResult::InProgress;
}

void UTask_Wander::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASeaCreatureAIController* AIController = Cast<ASeaCreatureAIController>(OwnerComp.GetAIOwner());

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	if (SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr || SeaCreature->MovementComponent == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const FSeaCreatureData* FishData = SeaCreature->GetData();
	if (FishData == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector Dir = SeaCreature->SteeringComp->ComputeWanderDir(DeltaSeconds);
	Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	Dir.Normalize();

	if (!Dir.IsNearlyZero())
	{
		SeaCreature->AddMovementInput(Dir, FishData->WanderSpeed);
		FRotator TargetRot = Dir.Rotation();
		SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRot, DeltaSeconds, 2.0f));
	}
	/*OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASeaCreatureAIController::MoveDirectionKey, Dir.GetSafeNormal());*/

}

EBTNodeResult::Type UTask_Wander::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
