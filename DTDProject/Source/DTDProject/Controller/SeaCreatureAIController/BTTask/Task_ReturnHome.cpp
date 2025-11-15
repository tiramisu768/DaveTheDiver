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
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (SeaCreature == nullptr || BlackboardComp == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    const FSeaCreatureData* FishData = SeaCreature->GetData();
    if (FishData == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    SeaCreature->MovementComponent->MaxSpeed = FishData->ReturnSpeed;
    SeaCreature->MovementComponent->Acceleration = FishData->Acceleration;

    Waypoints.Empty();

    const FVector StartLocation = SeaCreature->GetActorLocation();
    const FVector HomeLocation = BlackboardComp->GetValueAsVector(ASeaCreatureAIController::HomeLocationKey);

    const int32 NumWaypoints = FMath::RandRange(7, 10);
    for (int32 i = 1; i <= NumWaypoints; i++)
    {
        float Alpha = (float)i / (float)(NumWaypoints + 1);
        FVector Point = FMath::Lerp(StartLocation, HomeLocation, Alpha);
        Point += FMath::VRand() * FMath::FRandRange(100.f, 300.f);
        Waypoints.Add(Point);
    }

    Waypoints.Add(HomeLocation);

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

    if (Waypoints.Num() == 0)
    {
        BlackboardComp->SetValueAsBool(ASeaCreatureAIController::IsFarFromHomeKey, false);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    FVector CurrentTarget = Waypoints[0];

    //도착 판정을 할 때도 Z값을 무시하고 수평 거리만 계산한다.
    const float DistToHome2D = FVector::Dist2D(SeaCreature->GetActorLocation(), CurrentTarget);
    if (DistToHome2D < 150.f)
    {
        Waypoints.RemoveAt(0);
        if (Waypoints.Num() == 0) return;

        CurrentTarget = Waypoints[0];
    }

    //물고기는 HomeLocation의 XY 좌표를 향해서만 헤엄친다. Z축은 전혀 신경 쓰지 않는다.
    CurrentTarget.Z = SeaCreature->GetActorLocation().Z;
    
    FVector Dir = SeaCreature->SteeringComp->ComputeChaseDir(CurrentTarget);
    Dir += SeaCreature->SteeringComp->ComputeAvoidanceDir();
    Dir.Normalize();

    if (!Dir.IsNearlyZero())
    {
        const FSeaCreatureData* FishData = SeaCreature->GetData();
        SeaCreature->AddMovementInput(Dir,FishData->ReturnSpeed);
        FRotator TargetRotation = Dir.Rotation();
        SeaCreature->SetActorRotation(FMath::RInterpTo(SeaCreature->GetActorRotation(), TargetRotation, DeltaSeconds, 2.0f));
    }
}
