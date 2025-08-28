// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/Animation/SeaCreatureAnimInstance.h"
#include "GameFramework/Character.h"
#include "SeaCreature/SeaCreature.h"

void USeaCreatureAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Speed = 0.f; hasTarget = false; isInvestigating = false; isDead = false;
}

void USeaCreatureAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (ASeaCreature* OwningCharacter = Cast<ASeaCreature>(TryGetPawnOwner()))
	{
		Speed = OwningCharacter->GetVelocity().Size();
		GEngine->AddOnScreenDebugMessage(-6, 2.0f, FColor::Black, FString::Printf(TEXT("Speed: %f"), Speed));
		isDead = OwningCharacter->isDead();
	}
}
