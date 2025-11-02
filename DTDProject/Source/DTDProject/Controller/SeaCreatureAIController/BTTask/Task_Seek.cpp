// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Seek.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

UTask_Seek::UTask_Seek()
{
    bNotifyTick = true;
    BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UTask_Seek, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UTask_Seek::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    return EBTNodeResult::InProgress;
}

void UTask_Seek::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	if (SeaCreature == nullptr || TargetActor == nullptr || SeaCreature->SteeringComp == nullptr || SeaCreature->MovementComponent == nullptr)
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

	FVector Dir = SeaCreature->SteeringComp->ComputeSeekDir(TargetActor->GetActorLocation());
	Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	Dir.Normalize();

	if (!Dir.IsNearlyZero())
	{
		SeaCreature->MovementComponent->MaxSpeed = FishData->SeekSpeed;
		SeaCreature->AddMovementInput(Dir);
		FRotator TargetRotation = Dir.Rotation();
		SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
	}
}
