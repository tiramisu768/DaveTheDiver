// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/SeaCreature.h"
#include "MyRobo/MyRobo.h"
#include "ActorComponent/StateComponent/FishStateComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/SimpleDamageUI.h"
#include "Object/ObjectUI/DamagePopup.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/skeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
//#include "Kismet/GameplayStatics.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"

// Sets default values
ASeaCreature::ASeaCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> SeaCreatureDataTableFinder(TEXT("/Script/Engine.DataTable'Game/BluePrint/SeaCreature/Data/DT_SeaCreatureData'"));
	if (SeaCreatureDataTableFinder.Succeeded())
		SeaCreatureDataTable = SeaCreatureDataTableFinder.Object;

	GetCharacterMovement()->NavAgentProps.bCanSwim = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->GravityScale = 0.f;

	FishStateComponent = CreateDefaultSubobject<UFishStateComponent>(TEXT("FishStateComponent"));
	FishHPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FishHPBarWidget"));
	FishHPBarWidget->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> FishHPBarWidgetClassFinder(TEXT(""));
	if (FishHPBarWidgetClassFinder.Succeeded())
		FishHPBarWidget->SetWidgetClass(FishHPBarWidgetClassFinder.Class);
	FishHPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);


	static ConstructorHelpers::FObjectFinder<UAnimMontage>DeathFlapMontageObjectFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/SeaCreature/Animation/AM_Death.AM_Death'"));
	if (DeathFlapMontageObjectFinder.Succeeded())
		DeathFlapMontage = DeathFlapMontageObjectFinder.Object;
	CollectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectSphere"));
	CollectSphere->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageObjectFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/SeaCreature/Animation/AM_Attack_PinkShark.AM_Attack_PinkShark'"));
	if (AttackMontageObjectFinder.Succeeded())
		AttackMontage = AttackMontageObjectFinder.Object;

	AIControllerClass = ASeaCreatureAIController::StaticClass();
	//EAutoPossessAI
	//Disabled, //AIController사용안함
	//PlacedInWorld.//게임 시작시 배치되어있는 pawn은 AIController를 소유함
	//Spawned, //Spawn된 Pawn은 AIController를 소유함
	//PlacedInWorldOrSpawned, //게임 시작시 배치되어있는 Pawn과 Spawn된 Pawn은 AIController를 소유함
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SteeringComp = CreateDefaultSubobject<USeaCreatureSteeringComponent>(TEXT("SteeringComponent"));
}

// Called when the game starts or when spawned
void ASeaCreature::BeginPlay()
{
	Super::BeginPlay();
	
	//Data = SeaCreatureDataTable->Findero

	/*if (Data)
	{
		const auto& Stats = Data->Stats;

		SteeringComp->InitParams(
			Data
			Stats.WanderRadius,
			Stats.SlowRadius);
	}*/

	FishStateComponent->OnTakeDamage.BindLambda([this](float Percent)
		{
			////////다시하기///////////
			/*UFishHPBar* FishHPBarUI = Cast<UFishHPBar>(FishHPBarWidget->GetWidget());
			if (FishHPBarUI)
			{
				FishHPBarUI->SetDamageText(FishStateComponent->GetDamage());
			}*/
		}
	);

	CollectSphere->OnComponentBeginOverlap.AddDynamic(this, &ASeaCreature::OnCollectOverlap);
}

void ASeaCreature::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	/*if (SteeringComp)
	{
		if (IsAggressive)
			SteeringComp->Home
	}*/
}

// Called every frame
void ASeaCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	///////AI 하면 사라질 부분////////
	/*if (!FishStateComponent->IsDead())
		AddMovementInput(Forward);*/
}

// Called to bind functionality to input
void ASeaCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASeaCreature::SpawnSeaCreature()
{
	/*int index = FMath::RandHelper(StatDataNames.Num());
	FName SeaCreatureName = StatDataNames[index];
	FSeaCreatureData* Data = SeaCreatureDataTable->FindRow<FSeaCreatureData>(SeaCreatureName, TEXT("SeaCreatureDataTableDebug"));
	if (nullptr != Data)
	{
		GetWorld()->SpawnActor<ASeaCreature>(SpawnMonsterClass, GetActorLocation(), GetActorRotation(), FActorSpawnParameters());
		InitStat(*Data);
	}*/
}

void ASeaCreature::HitBy(float DamageAmount, const FHitResult& HitResult)
{
	if (FishStateComponent->IsDead() || HitbyMontage == nullptr)
		return;

	FishStateComponent->TakeDamage(DamageAmount);
	SpawnDamagePopup(DamageAmount);
	/*UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location,
		HitResult.Normal.Rotation(), true);*/

	//if (HitEffect)
	//{
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	//		GetWorld(),
	//		HitEffect,
	//		HitResult.Location,
	//		HitResult.Normal.Rotation(),   // 방향
	//		FVector(1.0f),                 // 스케일
	//		true                           // AutoDestroy
	//	);
	//}

	if (FishStateComponent->IsDead())
	{	
		Die();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.0f, FColor::Red, FString::Printf(TEXT("Not isDead")));
		PlayAnimMontage(HitbyMontage);
	}


}

void ASeaCreature::Die()
{
	//사망직전 파닥파닥 애님
	PlayAnimMontage(DeathFlapMontage);

	auto* CM = GetCharacterMovement();
	CM->SetMovementMode(MOVE_None);

	USkeletalMeshComponent* M = GetMesh();
	// 현재 프레임 고정
	M->bPauseAnims = true;
	M->SetComponentTickEnabled(false);

	//캡슐끄고, 메쉬의 콜리전을 Ragdoll로 설정
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	M->SetCollisionProfileName(TEXT("Ragdoll"));
	M->SetAllBodiesSimulatePhysics(false);
	M->SetSimulatePhysics(false);
	M->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	M->SetAllBodiesBelowSimulatePhysics(FName("Spine_006_077"), true, true);
	M->SetAllBodiesBelowPhysicsBlendWeight(FName("Spine_006_077"), 0.5f);

	// 물속 감쇠로 덜 출렁이게
	M->SetLinearDamping(2.5f);
	M->SetAngularDamping(3.5f);

	GetWorld()->GetTimerManager().SetTimer(DeathRotateTimerHandle, [this]()
		{
			RotateToDeadPose(0.01f);
		}, 0.01f, true
	);
	//////////10초 후 자동삭제로 하고 20초 내에 로보가 물고기에 부딪히면 수확 및 삭제////////
	//GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]()
	//	{
	//		Destroy();
	//	}, 10.0f, false
	//);
}

bool ASeaCreature::isDead()
{
	return FishStateComponent->IsDead();
}

void ASeaCreature::RotateToDeadPose(float DeltaTime)
{
	//서서히 0→Roll ~90도 보간
	const FRotator Target = FRotator(0.f, GetActorRotation().Yaw, 90.f);
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), Target, DeltaTime, 0.5f));
}

void ASeaCreature::EnableCollectTrigger(bool isEnable)
{
	CollectSphere->SetCollisionEnabled(isEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	CollectSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollectSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
}

void ASeaCreature::OnCollectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!FishStateComponent->IsDead()) return;
	if (AMyRobo* robo = Cast<AMyRobo>(OtherActor))
	{
		CollectSeaCreature();
	}
}

void ASeaCreature::CollectSeaCreature()
{
	EnableCollectTrigger(false);

	// 루팅 로직(아이템 지급) …

	// 사라지기(이펙트+사운드 후)
	SetLifeSpan(0.1f); // 또는 페이드/ Dissolve 후 Destroy
}

void ASeaCreature::Attack(AMyRobo* Target)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Monster Attack!"));
	if (FishStateComponent->IsDead() || AttackMontage == nullptr || Target == nullptr)
		return;
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
		return;
	FVector TargetDirection = Target->GetActorLocation() - GetActorLocation();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
	SetActorRotation(LookAtRotation);
	PlayAnimMontage(AttackMontage);
}

void ASeaCreature::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ASeaCreature::OnAttackMontageEnded);
	}
}

void ASeaCreature::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage || Montage == HitbyMontage)
	{
		OnAttackMontageEndedDelegate.ExecuteIfBound();
	}
}

void ASeaCreature::SpawnDamagePopup(float DamageAmount)
{
	/*float HeightOffset = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 0.3f;
	FVector SpawnLoc = GetActorLocation() + FVector(0.f, 0.f, HeightOffset);*/
	ADamagePopup* damagePopup = GetWorld()->SpawnActor<ADamagePopup>(SpawnDamagePopupClass, GetActorLocation(), GetActorRotation(), FActorSpawnParameters());
	if (nullptr == damagePopup) return;

	USimpleDamageUI* SimpleDamageUI = Cast<USimpleDamageUI>(damagePopup-> SimpleDamageWidget->GetWidget());
	if (SimpleDamageUI)
	{
		SimpleDamageUI->SetDamageText(FishStateComponent->GetDamage());
	}
	//////////////추후 오브젝트풀링으로 수정/////////////////
	damagePopup->HideDamagePopup();

	
}



