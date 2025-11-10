// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTService/Service_PerceptionUpdate.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "MyRobo/MyRobo.h"
#include "SeaCreature/SeaCreature.h"


UService_PerceptionUpdate::UService_PerceptionUpdate()
{
	NodeName = TEXT("Perception Update");
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UService_PerceptionUpdate, BlackboardKey), AActor::StaticClass());
}

void UService_PerceptionUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(AIController ? AIController->GetPawn() : nullptr);
	if (AIController == nullptr || SeaCreature == nullptr) return;


	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackboardComp) return;
	if (AMyRobo* TargetActor = Cast<AMyRobo>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey())))
		return;


	const FSeaCreatureData* FishData = SeaCreature->GetData();
	if (FishData == nullptr) return;

	UAIPerceptionComponent* PerceptionComp = AIController->GetAIPerceptionComponent();
	if (!PerceptionComp) return;

	//현재 시야에 감지된 모든 액터 정보를 가져온다
	TArray<AActor*> Sensed;
	PerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Sensed);

	AActor* NearestTarget = nullptr;
	float MinDistSq = TNumericLimits<float>::Max();

	for (AActor* Actor : Sensed)
	{
		if (Cast<AMyRobo>(Actor))
		{
			//지금은 필요없지만, 여러 대상을 감지했을 떄를 대비해 가장 가까운대상을 찾는다
			float DistSq = FVector::DistSquared(AIController->GetPawn()->GetActorLocation(), Actor->GetActorLocation());
			if (DistSq < FMath::Square(FishData->FleeDistance))
			{
				if (DistSq < MinDistSq)
				{
					MinDistSq = DistSq;
					NearestTarget = Actor;
				}
			}
		}
	}
	if (nullptr == NearestTarget) return;

	//찾은 결과를 블랙보드에 보고한다

		//NearestTarget이 유효하면 플레이어 액터가, 보이지 않으면 nullptr이 설정된다
	BlackboardComp->SetValueAsObject(GetSelectedBlackboardKey(), NearestTarget);
	const FVector HomeLocation = BlackboardComp->GetValueAsVector(TEXT("HomeLocation"));
	const FVector CurrentLocation = SeaCreature->GetActorLocation();
	const float DistanceFromHome = FVector::Dist(CurrentLocation, HomeLocation);

	BlackboardComp->SetValueAsFloat(TEXT("DistanceFromHome"), DistanceFromHome);

	FString DistMsg = FString::Printf(TEXT("DistanceFromHome: %.1f"), DistanceFromHome);
	GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Yellow, DistMsg);
}
