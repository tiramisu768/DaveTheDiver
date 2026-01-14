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
}

EBTNodeResult::Type UTask_Flee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
	const FSeaCreatureData* FishData = SeaCreature->GetData();
	SeaCreature->MovementComponent->MaxSpeed = FishData->FleeSpeed;
	SeaCreature->MovementComponent->Acceleration = FishData->Acceleration;
	return EBTNodeResult::InProgress;
}

void UTask_Flee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());

	if (BlackboardComp == nullptr || SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr)
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

	bool bFleeFinished = false;
	if (TargetActor == nullptr)
	{
		bFleeFinished = true;
	}
	else
	{
		float Dist = FVector::Dist(SeaCreature->GetActorLocation(), TargetActor->GetActorLocation());
		if (Dist > FishData->SafeDistance)
		{
			bFleeFinished = true;
		}
	}

	if (bFleeFinished)
	{
		BlackboardComp->ClearValue(ASeaCreatureAIController::TargetActorKey);

		const FVector HomeLocation = BlackboardComp->GetValueAsVector(ASeaCreatureAIController::HomeLocationKey);
		const float HomeReturnDistance = FishData->WanderRadius;
		const float CurrentDistance = FVector::Dist(SeaCreature->GetActorLocation(), HomeLocation);

		BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsFarFromHomeKey, CurrentDistance > HomeReturnDistance);

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
}
