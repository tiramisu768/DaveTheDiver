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
}

EBTNodeResult::Type UTask_ReturnHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("RETURNHOME TASK STARTED!"));
    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
    const FSeaCreatureData* FishData = SeaCreature->GetData();

    SeaCreature->MovementComponent->MaxSpeed = FishData->ReturnSpeed;
    SeaCreature->MovementComponent->Acceleration = FishData->Acceleration;

    return EBTNodeResult::InProgress;
}

void UTask_ReturnHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    ASeaCreature* SeaCreature = Cast<ASeaCreature>(OwnerComp.GetAIOwner()->GetPawn());
    if (BlackboardComp==nullptr || SeaCreature == nullptr || SeaCreature->SteeringComp == nullptr)
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

    const FVector HomeLocation = BlackboardComp->GetValueAsVector(ASeaCreatureAIController::HomeLocationKey);
    //도착 판정을 할 때도 Z값을 무시하고 수평 거리만 계산한다.
    const float DistToHome2D = FVector::Dist2D(SeaCreature->GetActorLocation(), HomeLocation);

    if (DistToHome2D < 100.f)
    {
        BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsFarFromHomeKey, false);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    //물고기는 HomeLocation의 XY 좌표를 향해서만 헤엄친다. Z축은 전혀 신경 쓰지 않는다.
    FVector HorizontalTarget = HomeLocation;
    HorizontalTarget.Z = SeaCreature->GetActorLocation().Z;

    FVector Dir = SeaCreature->SteeringComp->ComputeChaseDir(HorizontalTarget);
    Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
    Dir.Normalize();

    if (!Dir.IsNearlyZero())
    {
        SeaCreature->AddMovementInput(Dir,FishData->ReturnSpeed);
        FRotator TargetRotation = Dir.Rotation();
        SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
    }
}
