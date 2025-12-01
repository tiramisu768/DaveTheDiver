// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RangedAttackTrace.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API URangedAttackTrace : public UAnimNotify
{
	GENERATED_BODY()
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
