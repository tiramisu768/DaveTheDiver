// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTService/Service_PerceptionUpdate.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "MyRobo/MyRobo.h"
#include "SeaCreature/SeaCreature.h"


UService_PerceptionUpdate::UService_PerceptionUpdate()
{
	NodeName = TEXT("Perception Update");
}

void UService_PerceptionUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASeaCreatureAIController* AIController = Cast<ASeaCreatureAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) 
		return;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackboardComp) return;

	TArray<AActor*> Sensed;
	AIController->GetAIPerceptionComponent()->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Sensed);

	AActor* NearestTarget = nullptr;
	float MinDistSq = TNumericLimits<float>::Max();

	for (AActor* Actor : Sensed)
	{
		if (Cast<AMyRobo>(Actor))
		{
			float DistSq = FVector::DistSquared(AIController->GetPawn()->GetActorLocation(), Actor->GetActorLocation());
			if (DistSq < MinDistSq)
			{
				MinDistSq = DistSq;
				NearestTarget = Actor;
			}

		}
	}

	BlackboardComp->SetValueAsObject(ASeaCreatureAIController::TargetActorKey, NearestTarget);
	const FVector HomeLocation = BlackboardComp->GetValueAsVector(ASeaCreatureAIController::HomeLocationKey);
	const float DistanceFromHome = FVector::Dist(AIController->GetPawn()->GetActorLocation(), HomeLocation);
	BlackboardComp->SetValueAsFloat(TEXT("DistanceFromHome"), DistanceFromHome);
}
