// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRobo/Animation/Notify/AttackTrace.h"
#include "Interface/AttackTraceNotify/AttackTraceNotify.h"
#include "GameFramework/Character.h"

void UAttackTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character != nullptr && Character->GetMesh() != MeshComp)
		return;

	IAttackTraceNotify* AttackCharacter = Cast<IAttackTraceNotify>(MeshComp->GetOwner());
	if (!AttackCharacter)
		return;

	AttackCharacter->AttackTrace();

	//질문. 자동생성된 코드인데 위의 코드와는 단순한 스타일 차이인가?
	//if (MeshComp && MeshComp->GetOwner())
	//{
	//	AActor* Owner = MeshComp->GetOwner();
	//	if (Owner)
	//	{
	//		// Perform attack trace logic here
	//		// For example, call a function on the owner to handle the attack trace
	//		// Owner->AttackTrace();
	//	}
	//}
}
