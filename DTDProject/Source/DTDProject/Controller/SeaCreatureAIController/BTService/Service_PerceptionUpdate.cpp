// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTService/Service_PerceptionUpdate.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyRobo/MyRobo.h"
#include "BehaviorTree/BlackboardComponent.h"

UService_PerceptionUpdate::UService_PerceptionUpdate()
{
	NodeName = TEXT("Perception Update");
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = false;
}

void UService_PerceptionUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	UAIPerceptionComponent* PerceptionComp = AIController->FindComponentByClass<UAIPerceptionComponent>();
	if (!PerceptionComp) return;

	//로봇 갱신
	TArray<AActor*> Sensed;
	PerceptionComp->GetCurrentlyPerceivedActors(nullptr, Sensed);

	AMyRobo* Robo = nullptr;
	float Dist = -1.f;
	

	for (AActor* Actor : Sensed)
	{
		Robo = Cast<AMyRobo>(Actor);
		if (Robo)
		{
			Dist = FVector::Dist(Pawn->GetActorLocation(), Robo->GetActorLocation());
			break;
		}
	}

	if (Robo)
	{
		float LoseTargetDist = BlackboardComp->GetValueAsFloat(TEXT("LoseTargetDist"));
		if(LoseTargetDist >0.f && Dist > LoseTargetDist)
		{
			//타겟 놓침
			BlackboardComp->ClearValue(TEXT("TargetActor"));
			BlackboardComp->SetValueAsBool(TEXT("HasThreat"), false);
			BlackboardComp->SetValueAsFloat(TEXT("DistanceToTarget"), -1.f);
		}
		else
		{
			//타겟 유지
			BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Robo);
			BlackboardComp->SetValueAsBool(TEXT("HasThreat"), true);
			BlackboardComp->SetValueAsFloat(TEXT("DistanceToTarget"), Dist);
		}
	}
	else
	{
		//perception 전혀 못 함
		BlackboardComp->ClearValue(TEXT("TargetActor"));
		BlackboardComp->SetValueAsBool(TEXT("HasThreat"), false);
		BlackboardComp->SetValueAsFloat(TEXT("DistanceToTarget"), -1.f);
	}

	//Home 갱신
	FVector Home = BlackboardComp->GetValueAsVector(TEXT("HomeLocation"));
	const FVector Target = Pawn->GetActorLocation();

	// Home이 아직 유효하게 세팅되지 않았다면(보통 FLT_MAX 벡터)
	if (!FAISystem::IsValidLocation(Home))
	{
		// 첫 틱에 보정: 현재 위치를 Home으로 세팅
		BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), Target);
		BlackboardComp->SetValueAsFloat(TEXT("DistanceFromHome"), 0.f);
		return;
	}

	// 거리 계산 + 무한대/NaN 가드
	const float DistHome = FVector::Dist(Target, Home);
	BlackboardComp->SetValueAsFloat(TEXT("DistanceFromHome"),FMath::IsFinite(DistHome) ? DistHome : 0.f);
}
