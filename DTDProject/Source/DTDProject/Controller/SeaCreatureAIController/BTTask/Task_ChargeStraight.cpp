// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_ChargeStraight.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"

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

	FVector Goal = BlackboardComp->GetValueAsVector(ASeaCreatureAIController::ChaseTargetLocationKey);

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

	FVector Goal = BlackboardComp->GetValueAsVector(ASeaCreatureAIController::ChaseTargetLocationKey);
	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(ASeaCreatureAIController::TargetActorKey));

	if (Goal.IsNearlyZero() || Target == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	bool bAttackReady = false;

	FVector StartLocation = SeaCreature->SphereComponent-> GetComponentLocation();
	FVector EndLocation = StartLocation + (Goal - StartLocation).GetSafeNormal() * SeaCreature->GetData()->ChaseSpeed * DeltaSeconds;

	FHitResult HitResult;
	FCollisionShape  SphereShape = FCollisionShape::MakeSphere(SeaCreature->SphereComponent->GetScaledSphereRadius());
	FCollisionQueryParams CollisionParams(TEXT("ChargeSweep"),false, SeaCreature);
	CollisionParams.bFindInitialOverlaps = false;

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel5,
		SphereShape,
		CollisionParams
	);

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 0.1f, 0, 1.0f);

	if (bHit && HitResult.GetActor() == Target)
	{
		bAttackReady = true;
	}

	if (!bAttackReady && FVector::DistSquared(StartLocation, Goal) <= FMath::Square(SeaCreature->GetData()->AttackRange))
	{
		bAttackReady = true;
	}

	if (bAttackReady)
	{
		BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsReadyToAttackKey,true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector Dir = Goal-StartLocation;
	Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	Dir.Normalize();

	if (!Dir.IsNearlyZero())
	{
		SeaCreature->AddMovementInput(Dir, SeaCreature->GetData()->ChaseSpeed);
		FRotator TargetRotation = Dir.Rotation();
		SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
	}
}
