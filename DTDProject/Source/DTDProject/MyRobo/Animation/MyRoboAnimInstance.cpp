// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRobo/Animation/MyRoboAnimInstance.h"
#include "MyRobo/MyRobo.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include <Controller/MyCharacterController/MyCharacterController.h>

void UMyRoboAnimInstance::NativeInitializeAnimation()
{
	OwnerRobo = Cast<AMyRobo>(TryGetPawnOwner());
	if (OwnerRobo)
	{
		//MovementComponent = OwnerRobo->GetCharacterMovement();
		MovementComponent = OwnerRobo->FindComponentByClass<UCharacterMovementComponent>();
	}

}

void UMyRoboAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerRobo)
	{
		isMove = MovementComponent->Velocity.Size() > 0.0f;
		//MoveVelocity = MovementComponent->Velocity;
		MoveVelocity = MovementComponent->Velocity.Size2D();
		isDash = OwnerRobo->GetMovementComponent()->IsSwimming();
		isSwimming = OwnerRobo->GetMovementComponent()->IsSwimming();
		//isEquip = OwnerRobo->GetisEquip();
		/*AMyCharacterController* OwnerController = Cast< AMyCharacterController>(OwnerCharacter->GetController());
		if (nullptr != OwnerController)
			isMoveInput = OwnerController->GetIsMoveInput();*/
	}
}
