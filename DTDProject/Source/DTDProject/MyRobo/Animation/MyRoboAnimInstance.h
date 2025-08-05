// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyRoboAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DTDPROJECT_API UMyRoboAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AMyRobo> OwnerRobo;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isMove;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector MoveVelocity;
	//UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//bool isJump;
	//UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//bool isMoveInput;
	//UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//bool isEquip;
public:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
