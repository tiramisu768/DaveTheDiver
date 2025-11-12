// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_Flee.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

UTask_Flee::UTask_Flee()
{
	bNotifyTick = true;
	NodeName = TEXT("Flee From Target");
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UTask_Flee, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UTask_Flee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("FLEE TASK STARTED!"));
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("FleeStartTime"), GetWorld()->GetTimeSeconds());
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	const FSeaCreatureData* FishData = SeaCreature->GetData();
	SeaCreature->MovementComponent->MaxSpeed = FishData->FleeSpeed;
	SeaCreature->MovementComponent->Acceleration = FishData->Acceleration;
	return EBTNodeResult::InProgress;
}

void UTask_Flee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());

	if (TargetActor == nullptr || SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASeaCreatureAIController::MoveDirectionKey,FVector::ZeroVector);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	const FSeaCreatureData* FishData = SeaCreature->GetData();
	if (FishData == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	float Dist = FVector::Dist(SeaCreature->GetActorLocation(), TargetActor->GetActorLocation());
	if (Dist > FishData->FleeSafeDistance)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(ASeaCreatureAIController::TargetActorKey);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASeaCreatureAIController::MoveDirectionKey, FVector::ZeroVector);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector Dir = SeaCreature->SteeringComp->ComputeFleeDir(TargetActor->GetActorLocation());
	Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	Dir.Normalize();

	if (!Dir.IsNearlyZero())
	{
		SeaCreature->AddMovementInput(Dir, FishData->FleeSpeed);
		FRotator TargetRotation = Dir.Rotation();
		SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
	}
	/*OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASeaCreatureAIController::MoveDirectionKey, Dir.GetSafeNormal());*/
}

EBTNodeResult::Type UTask_Flee::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
