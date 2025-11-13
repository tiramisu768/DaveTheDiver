// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTTask/Task_ReturnHome.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SeaCreature/SeaCreature.h"
#include "SeaCreature/SeaCreatureStateType.h"
#include "GameFramework/FloatingPawnMovement.h"

UTask_ReturnHome::UTask_ReturnHome()
{
    bNotifyTick = true;
    NodeName = TEXT("Return Home");

    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UTask_ReturnHome, BlackboardKey));
}

EBTNodeResult::Type UTask_ReturnHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
    const FSeaCreatureData* FishData = SeaCreature->GetData();
    SeaCreature->MovementComponent->MaxSpeed = FishData->ReturnSpeed;
    SeaCreature->MovementComponent->Acceleration = FishData->Acceleration;
    return EBTNodeResult::InProgress;
}

void UTask_ReturnHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
    if (SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr || SeaCreature->MovementComponent == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASeaCreatureAIController::TargetActorKey) != nullptr)
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

    FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ASeaCreatureAIController::HomeLocationKey);
	float HomeReturnDist = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ASeaCreatureAIController::HomeReturnDistKey);
    float Dist = FVector::Dist(SeaCreature->GetActorLocation(), HomeLocation);
    
    if (Dist < HomeReturnDist)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASeaCreatureAIController::MoveDirectionKey, FVector::ZeroVector);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    FVector Dir = SeaCreature->SteeringComp->ComputeChaseDir(HomeLocation);
    Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
    Dir.Normalize();

    if (!Dir.IsNearlyZero())
    {
        SeaCreature->AddMovementInput(Dir,FishData->ReturnSpeed);
        FRotator TargetRotation = Dir.Rotation();
        SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
    }

}

EBTNodeResult::Type UTask_ReturnHome::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::Aborted;
}
