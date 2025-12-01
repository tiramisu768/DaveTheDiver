// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRobo/Animation/Notify/RangedAttackTrace.h"
#include "MyRobo/MyRobo.h"

void URangedAttackTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AMyRobo* Robo = Cast<AMyRobo>(MeshComp->GetOwner());
	if (Robo)
	{
		Robo->FireProjectile();
	}
}
