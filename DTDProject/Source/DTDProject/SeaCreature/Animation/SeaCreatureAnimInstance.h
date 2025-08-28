// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SeaCreatureAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API USeaCreatureAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion", meta = (AllowPrivateAccess = "true"))
	float Speed{ 0.0f };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion", meta = (AllowPrivateAccess = "true"))
	bool hasTarget{ false }; // 플레이어 감지 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Locomotion", meta = (AllowPrivateAccess = "true"))
	bool isInvestigating{ false }; // 놓친 직후 물음표 띄우고 머무는 상태
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool isDead{ false };
public:
    virtual void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

    /** 외부(캐릭터/AI)에서 세팅 */
   /* UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetHasTarget(bool b) { hasTarget = b; }

    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetInvestigating(bool b) { isInvestigating = b; }

    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetDead(bool b) { isDead = b; }*/
};
