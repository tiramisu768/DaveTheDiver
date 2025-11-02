// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/Animation/SeaCreatureAnimInstance.h"
#include "SeaCreature/SeaCreature.h"
#include "ActorComponent/StateComponent/FishStateComponent.h"

void USeaCreatureAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Speed = 0.f; hasTarget = false; isInvestigating = false; isDead = false;
}

void USeaCreatureAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (ASeaCreature* OwningPawn = Cast<ASeaCreature>(TryGetPawnOwner()))
	{
		Speed = OwningPawn->GetVelocity().Size();
		if (OwningPawn->FishStateComponent)
		{
			// 2. 유효할 때만 isDead()를 호출합니다.
			isDead = OwningPawn->isDead();
		}
		else
		{
			// 만약 컴포넌트가 없다면, 안전하게 false로 처리합니다.
			// (에디터 프리뷰 상태 등에서 이럴 수 있습니다)
			isDead = false;
		}
	}
}
