// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/TerritorialAttackStrategy.h"
#include "SeaCreature/SeaCreature.h"
#include "MyRobo/MyRobo.h"

void UTerritorialAttackStrategy::ExecuteAttack(ASeaCreature* Attacker, AMyRobo* Target)
{
	if (!CanExecuteAttack(Attacker, Target))
	{
		return;
	}

	Attacker->GetMesh()->GetAnimInstance()->Montage_Play(Attacker->GetData()->AttackMontage);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Territorial Attack!"));
}
