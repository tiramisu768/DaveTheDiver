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
	NodeName = TEXT("Seek Target");
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UTask_Seek, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UTask_Seek::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FSeekTaskMemory* MyMemory = reinterpret_cast<FSeekTaskMemory*>(NodeMemory);
	MyMemory->State = ESeekState::None;
	MyMemory->SearchTimer = 0.0f;

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (TargetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	MyMemory->GoalLocation = TargetActor->GetActorLocation();
	MyMemory->State = ESeekState::MovingToGoal;

	return EBTNodeResult::InProgress;
}

void UTask_Seek::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FSeekTaskMemory* MyMemory = reinterpret_cast<FSeekTaskMemory*>(NodeMemory);

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

	switch (MyMemory->State)
	{
	case ESeekState::MovingToGoal:
	{
		// 공격 목표 지점에 도달했는지 확인
		if (FVector::DistSquared(SeaCreature->GetActorLocation(), MyMemory->GoalLocation) < FMath::Square(AcceptanceRadius))
		{
			//도착했다면 탐색 상태로 전환
			MyMemory->State = ESeekState::SearchingAtGoal;
			MyMemory->SearchTimer = 0.0f;
			SeaCreature->GetMovementComponent()->Velocity = FVector::ZeroVector;
		}
		else
		{
			//아직 이동 중이면 목표를 향해 계속 이동
			FVector Dir = SeaCreature->SteeringComp->ComputeSeekDir(MyMemory->GoalLocation);
			Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
			Dir.Normalize();

			if (!Dir.IsNearlyZero())
			{
				SeaCreature->MovementComponent->MaxSpeed = FishData->SeekSpeed;
				SeaCreature->AddMovementInput(Dir);
				FRotator TargetRotation = Dir.Rotation();
				SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 5.0f));
			}
		}
		break;
	}

	case ESeekState::SearchingAtGoal:
		MyMemory->SearchTimer += DeltaSeconds;

		if (MyMemory->SearchTimer >= SearchDuration)
		{
			MyMemory->GoalLocation = TargetActor->GetActorLocation();
			MyMemory->State = ESeekState::MovingToGoal;
		}
		else
		{
			FVector LookAtDir = TargetActor->GetActorLocation() - SeaCreature->GetActorLocation();
			LookAtDir.Z = 0;
			FRotator TargetRotation = LookAtDir.Rotation();
			SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 3.0f));
		}
		break;
	}
}

uint16 UTask_Seek::GetInstanceMemorySize() const
{
	return sizeof(FSeekTaskMemory);
}
