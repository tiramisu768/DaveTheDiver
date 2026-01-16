// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/SeaCreature.h"
#include "MyRobo/MyRobo.h"
#include "ActorComponent/StateComponent/FishStateComponent.h"
#include "ActorComponent/InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/SimpleDamageUI.h"
#include "Object/ObjectUI/DamagePopup.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/skeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
//#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Controller/SeaCreatureAIController/SeaCreatureAIController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Controller/SeaCreatureAIController/SeaCreatureSteeringComponent.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "SeaCreature/AttackStrategy.h"
#include "SeaCreature/AggressiveAttackStrategy.h"
#include "SeaCreature/TerritorialAttackStrategy.h"

ASeaCreature::ASeaCreature()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;

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

	////////////GI
	/*if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GameInstance->OnGameEnded.AddUObject(this, &ASeaCreature::StopAIBehavior);
	}*/
	
	Data = SeaCreatureDataHandle.GetRow<FSeaCreatureData>(TEXT("SeaCreature BeginPlay"));

	if (Data)
	{
		if (FishStateComponent)
		{
			FishStateComponent->SetMaxHP(Data->MaxHP);
		}

		if (Data->AnimClass)
		{
			GetMesh()->SetAnimInstanceClass(Data->AnimClass);

			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			{
				AnimInstance->OnMontageEnded.AddDynamic(this, &ASeaCreature::OnMontageEnded_Handler);
			}
		}

		if (MovementComponent)
		{
			MovementComponent->Acceleration = Data->Acceleration;
			MovementComponent->Deceleration = Data->Acceleration;
		}

		switch (Data->Disposition)
		{
		case ESeaDisposition::Aggressive:
			AttackStrategy = NewObject<UAggressiveAttackStrategy>(this);
			HomeReturnDist = Data->WanderRadius - 300.f;
			break;
		case ESeaDisposition::Territorial:
			AttackStrategy = NewObject<UTerritorialAttackStrategy>(this);
			HomeReturnDist = Data->WanderRadius - 400.f;
			break;
		case ESeaDisposition::Passive:
			AttackStrategy = nullptr;
			HomeReturnDist = Data->WanderRadius - 100.f;
			break;
		}
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

void ASeaCreature::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	///////////GI
	/*if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GameInstance->OnGameEnded.RemoveAll(this);
	}*/
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

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* Brain = AIController->GetBrainComponent())
		{
			Brain->PauseLogic(TEXT("Hit"));
		}
	}

	FishStateComponent->TakeDamage(DamageAmount,HitResult);
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
		GEngine->AddOnScreenDebugMessage(-2, 5.0f, FColor::Red, FString::Printf(TEXT("Not dead yet")));
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
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	if (AttackStrategy)
	{
		AttackStrategy->ExecuteAttack(this, Target);
	}
}

void ASeaCreature::AttackTrace()
{
	TArray<AActor*> HitActors;
	TArray<FHitResult> HitResults;
	bool isHit = UKismetSystemLibrary::BoxTraceMulti(
		this,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 100.0f,
		FVector(50.0f, 50.0f, 50.0f),
		FRotator::ZeroRotator,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5),
		false,
		{},
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

	if (isHit)
	{
		for (const FHitResult& result : HitResults)
		{
			AMyRobo* Robo = Cast<AMyRobo>(result.GetActor());
			if (Robo != nullptr && !HitActors.Contains(Robo))
			{
				HitActors.Add(Robo); //중복피격방지
				Robo->HitBy(this, result);
			}
		}
	}
}

void ASeaCreature::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASeaCreature::OnMontageEnded_Handler(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == Data->HitbyMontage)
	{
		OnHitMontageEnded(bInterrupted);
	}
	else if (Montage == Data->AttackMontage)
	{
		OnAttackMontageEnded(bInterrupted);
	}
}

void ASeaCreature::OnHitMontageEnded(bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			if (UBrainComponent* Brain = AIController->GetBrainComponent())
			{
				Brain->ResumeLogic(TEXT("Hit"));
			}
		}
	}
}

void ASeaCreature::OnAttackMontageEnded(bool bInterrupted)
{
	OnAttackMontageEndedDelegate.ExecuteIfBound();
}



void ASeaCreature::SpawnDamagePopup(float DamageAmount)
{
	float HeightOffset = SphereComponent->GetScaledSphereRadius() + 0.3f;
	FVector SpawnLoc = GetActorLocation() + FVector(0.f, 0.f, HeightOffset);
	ADamagePopup* damagePopup = GetWorld()->SpawnActor<ADamagePopup>(SpawnDamagePopupClass, SpawnLoc, GetActorRotation(), FActorSpawnParameters());

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

void ASeaCreature::StopAIBehavior()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->StopLogic(TEXT("Player is dead"));
		}

		AIController->StopMovement();
	}

	if (MovementComponent)
	{
		MovementComponent->Velocity = FVector::ZeroVector;
	}
}

FSeaCreatureData::FSeaCreatureData()
{
}
