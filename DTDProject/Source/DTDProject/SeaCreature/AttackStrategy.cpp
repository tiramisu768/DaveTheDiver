// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/AttackStrategy.h"
#include "SeaCreature/SeaCreature.h"
#include "MyRobo/MyRobo.h"
#include "Animation/AnimInstance.h"

bool UAttackStrategy::CanExecuteAttack(ASeaCreature* Attacker, AMyRobo* Target)
{
	if (Attacker == nullptr || Target == nullptr) return false;

	const FSeaCreatureData* Data = Attacker->GetData();
	UAnimInstance* AnimInstance = Attacker->GetMesh() ? Attacker->GetMesh()->GetAnimInstance() : nullptr;

	if (Data == nullptr || Data->AttackMontage == nullptr || AnimInstance == nullptr) return false;

	if (AnimInstance->Montage_IsPlaying(Data->AttackMontage)) return false;

	return true;
}

void UAttackStrategy::ExecuteAttack(ASeaCreature* Attacker, AMyRobo* Target)
{
	if (Attacker == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AttackStrategy: Attacker is null"));
	}
}
