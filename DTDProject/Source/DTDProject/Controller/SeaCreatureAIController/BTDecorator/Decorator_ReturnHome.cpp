// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SeaCreatureAIController/BTDecorator/Decorator_ReturnHome.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SeaCreature/SeaCreature.h"

UDecorator_ReturnHome::UDecorator_ReturnHome()
{
	NodeName = TEXT("CanReturnHome (DistanceFromHome > HomeReturnDist)");
}

bool UDecorator_ReturnHome::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ASeaCreature* SeaCreature = Cast<ASeaCreature>(Owner->GetPawn());

	if (!Owner || !BlackboardComp || !SeaCreature) return false;

	const float DistFromHome = BlackboardComp->GetValueAsFloat(TEXT("DIstanceFromHome"));
	const float HomeReturnDist = SeaCreature->GetHomeReturnDist();

	return DistFromHome > HomeReturnDist;
}
