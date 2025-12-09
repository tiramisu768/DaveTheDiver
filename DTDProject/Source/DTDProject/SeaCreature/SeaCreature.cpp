// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/SeaCreature.h"
#include "MyRobo/MyRobo.h"
#include "ActorComponent/StateComponent/FishStateComponent.h"
#include "ActorComponent/InventoryComponent.h"
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
#include "GameFramework/FloatingPawnMovement.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

ASeaCreature::ASeaCreature()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComponent;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Mesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));

	if (MovementComponent)
	{
		MovementComponent->Acceleration = 1500.0f;
		MovementComponent->Deceleration = 1500.0f;
		MovementComponent->TurningBoost = 8.0f;
	}

	FishStateComponent = CreateDefaultSubobject<UFishStateComponent>(TEXT("FishStateComponent"));
	
	static ConstructorHelpers::FObjectFinder<UDataTable> SeaCreatureDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/BluePrint/SeaCreature/Data/DT_SeaCreatureStat.DT_SeaCreatureStat'"));
	if (SeaCreatureDataTableFinder.Succeeded())
		SeaCreatureDataTable = SeaCreatureDataTableFinder.Object;

	static ConstructorHelpers::FClassFinder<ADamagePopup> DamageWidgetClassFinder(TEXT("/Game/BluePrint/SeaCreature/UI/BP_DamagePopup.BP_DamagePopup_C"));
	if (DamageWidgetClassFinder.Succeeded())
		SpawnDamagePopupClass = DamageWidgetClassFinder.Class;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>DeathFlapMontageObjectFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/SeaCreature/Animation/AM_Death.AM_Death'"));
	if (DeathFlapMontageObjectFinder.Succeeded())
		DeathFlapMontage = DeathFlapMontageObjectFinder.Object;

	CollectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectSphere"));
	CollectSphere->SetupAttachment(RootComponent);

	//move to data
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageObjectFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/SeaCreature/Animation/AM_Attack_PinkShark.AM_Attack_PinkShark'"));
	if (AttackMontageObjectFinder.Succeeded())
		AttackMontage = AttackMontageObjectFinder.Object;*/

	static ConstructorHelpers::FClassFinder<AAIController> AIControllerClassFinder(TEXT("/Game/BluePrint/SeaCreature/AI/BP_SeaCreatureAIController.BP_SeaCreatureAIController_C"));
	if (AIControllerClassFinder.Succeeded())
	{
		AIControllerClass = AIControllerClassFinder.Class;
	}
	//EAutoPossessAI
	//Disabled, //AIController사용안함
	//PlacedInWorld.//게임 시작시 배치되어있는 pawn은 AIController를 소유함
	//Spawned, //Spawn된 Pawn은 AIController를 소유함
	//PlacedInWorldOrSpawned, //게임 시작시 배치되어있는 Pawn과 Spawn된 Pawn은 AIController를 소유함
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SteeringComp = CreateDefaultSubobject<USeaCreatureSteeringComponent>(TEXT("SteeringComponent"));
	//GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
}

// Called when the game starts or when spawned
void ASeaCreature::BeginPlay()
{
	Super::BeginPlay();
	
	Data = SeaCreatureDataTable->FindRow<FSeaCreatureData>(RowName, TEXT(""));

	if (Data)
	{
		if (Data->AnimClass)
		{
			GetMesh()->SetAnimInstanceClass(Data->AnimClass);
		}

		if (MovementComponent)
		{
			MovementComponent->Acceleration = Data->Acceleration;
			MovementComponent->Deceleration = Data->Acceleration;
		}

		//공격성물고기는 900-300, 회피성물고기는 900-100
		HomeReturnDist = Data->IsAggressive ? Data->WanderRadius - 300.f : Data->WanderRadius - 100.f;

		SteeringComp->InitParams(Data->WanderRadius, Data->SlowRadius);
	}

	ASeaCreatureAIController* AIController = Cast<ASeaCreatureAIController>(GetController());
	if (AIController)
	{
		AIController->PlayBehaviorTree(this);
	}

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
	EnableCollectTrigger(false);
}

// Called every frame
void ASeaCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	if (FishStateComponent->IsDead() || Data->HitbyMontage == nullptr)
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
		if (Data->HitbyMontage && Mesh && Mesh->GetAnimInstance())
		{
			Mesh->GetAnimInstance()->Montage_Play(Data->HitbyMontage);
		}
	}


}

void ASeaCreature::Die()
{
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		AICon->StopMovement();
		if (AICon->BrainComponent)
		{
			AICon->BrainComponent->StopLogic("Dead");
		}
		AICon->UnPossess();
	}

	bUseControllerRotationYaw = false;

	if (UAIPerceptionComponent* Perception = FindComponentByClass<UAIPerceptionComponent>())
	{
		Perception->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
		Perception->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
	}

	EnableCollectTrigger(true);
	//사망직전 파닥파닥 애님
	if (DeathFlapMontage && Mesh && Mesh->GetAnimInstance())
	{
		Mesh->GetAnimInstance()->Montage_Play(DeathFlapMontage);
	}

	// 이동 컴포넌트를 비활성화하여 움직임을 멈춘다
	if (MovementComponent)
	{
		MovementComponent->Deactivate();
	}

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
	CollectSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);
	
}

void ASeaCreature::OnCollectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!FishStateComponent->IsDead()) return;

	if (AMyRobo* robo = Cast<AMyRobo>(OtherActor))
	{
		EnableCollectTrigger(false);
		
		if (DeathRotateTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(DeathRotateTimerHandle);
			DeathRotateTimerHandle.Invalidate();
		}

		robo->CollectSeaCreature(this);
	}
}

void ASeaCreature::Attack(AMyRobo* Target)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Monster Attack!"));
	if (FishStateComponent->IsDead() || Data->AttackMontage == nullptr || Target == nullptr)
		return;
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Data->AttackMontage))
		return;
	FVector TargetDirection = Target->GetActorLocation() - GetActorLocation();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
	SetActorRotation(LookAtRotation);
	if (Data->AttackMontage && Mesh && Mesh->GetAnimInstance())
	{
		Mesh->GetAnimInstance()->Montage_Play(Data->AttackMontage);
	}
}

void ASeaCreature::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 1. Mesh 포인터가 유효한지 먼저 확인합니다.
	if (Mesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASeaCreature::PostInitializeComponents - Mesh component is NULL for %s!"), *GetName());
		return; // Mesh가 없으면 더 이상 진행하지 않고 함수를 종료합니다.
	}

	// 2. Mesh가 유효하다는 것이 보장된 상태에서 AnimInstance를 가져옵니다.
	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ASeaCreature::OnAttackMontageEnded);
	}
	else
	{
		// AnimInstance가 없는 경우도 로그를 남겨서 디버깅을 돕습니다.
		UE_LOG(LogTemp, Warning, TEXT("ASeaCreature::PostInitializeComponents - AnimInstance is NULL for %s."), *GetName());
	}
}

void ASeaCreature::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == Data->AttackMontage || Montage == Data->HitbyMontage)
	{
		OnAttackMontageEndedDelegate.ExecuteIfBound();
	}
}

void ASeaCreature::SpawnDamagePopup(float DamageAmount)
{
	/*float HeightOffset = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 0.3f;
	FVector SpawnLoc = GetActorLocation() + FVector(0.f, 0.f, HeightOffset);*/
	ADamagePopup* damagePopup = GetWorld()->SpawnActor<ADamagePopup>(SpawnDamagePopupClass, GetActorLocation(), GetActorRotation(), FActorSpawnParameters());

	if(damagePopup)
	{
		if (damagePopup->SimpleDamageWidget)
		{
			USimpleDamageUI* SimpleDamageUI = Cast<USimpleDamageUI>(damagePopup->SimpleDamageWidget->GetWidget());
			if (SimpleDamageUI)
			{
				SimpleDamageUI->SetDamageText(DamageAmount);
			}
		}
	}
}

FSeaCreatureData::FSeaCreatureData()
{
}
