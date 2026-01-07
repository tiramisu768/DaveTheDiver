// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/AggressiveAttackStrategy.h"
#include "SeaCreature/SeaCreature.h"
#include "MyRobo/MyRobo.h"
#include "Animation/AnimInstance.h"

void UAggressiveAttackStrategy::ExecuteAttack(ASeaCreature* Attacker, AMyRobo* Target)
{
	if (!CanExecuteAttack(Attacker, Target))
	{
		return;
	}

	FVector TargetDirection = Target->GetActorLocation() - Attacker->GetActorLocation();
	FRotator LookAtRotation = TargetDirection.Rotation();
	Attacker->SetActorRotation(LookAtRotation);

	Attacker->GetMesh()->GetAnimInstance()->Montage_Play(Attacker->GetData()->AttackMontage);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Aggressive Attack!"));
}
