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
	return EBTNodeResult::InProgress;
}

void UTask_Flee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());

	const FSeaCreatureData* FishData = SeaCreature->GetData();
	if (FishData == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 1. 블랙보드에서 현재 타겟과 마지막으로 알려진 위치를 가져옵니다.
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	FVector FleeFromLocation;
	bool bHasValidLocation = false;

	if (TargetActor)
	{
		// 1a. 타겟이 보이면, 그 위치를 도망갈 기준으로 삼고, '마지막 위치'를 계속 갱신합니다.
		FleeFromLocation = TargetActor->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownTargetLocation"), FleeFromLocation);
		bHasValidLocation = true;
	}
	else
	{
		// 1b. 타겟이 안 보이면, '마지막으로 봤던 위치'를 기준으로 삼습니다.
		if(OwnerComp.GetBlackboardComponent()->IsVectorValueSet(TEXT("LastKnownTargetLocation")))
		{
			FleeFromLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("LastKnownTargetLocation"));
			bHasValidLocation = true;
		}
	}

	// 2. 도망갈 유효한 위치가 없으면 태스크를 실패 처리합니다.
	if (!bHasValidLocation)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FLEE FAILED: No valid location to flee from."));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 3. 현재 위치와 도망 기준점과의 거리를 계산합니다.
	const float CurrentDistanceSq = FVector::DistSquared(SeaCreature->GetActorLocation(), FleeFromLocation);

	// 4. 안전 거리에 도달했는지 확인합니다.
	if (CurrentDistanceSq > FMath::Square(FishData->FleeSafeDistance))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("FLEE SUCCEEDED: Reached safe distance."));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 5. 아직 안전 거리에 도달하지 못했다면, 계속 도망갑니다.
	FVector Dir = SeaCreature->SteeringComp->ComputeFleeDir(FleeFromLocation);
	Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
	Dir.Normalize();

	if (!Dir.IsNearlyZero())
	{
		SeaCreature->MovementComponent->MaxSpeed = FishData->FleeSpeed;
		SeaCreature->AddMovementInput(Dir);
		FRotator TargetRotation = Dir.Rotation();
		SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
	}
}

EBTNodeResult::Type UTask_Flee::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
