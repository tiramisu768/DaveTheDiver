// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRobo/MyRobo.h"
#include "Controller/MyCharacterController/MyCharacterController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "UI/RoboHPBarUI.h"		
#include "UI/LongPressUI.h"		
#include "UI/MainUI.h"
#include "ActorComponent/StateComponent/RoboComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "BuoyancyComponent.h"
#include "WaterBodyComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SeaCreature/SeaCreature.h"
#include "Engine/OverlapResult.h"
#include "Interface/InteractionObject.h"
#include "Weapon/Weapon.h"
#include "Object/RandomBox.h"


// Sets default values
AMyRobo::AMyRobo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#pragma region Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	BodyComponent = GetMesh();
	BodyComponent->SetupAttachment(GetRootComponent());

	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	//WeaponComponent->SetupAttachment(BodyComponent, FName(TEXT("Weapon")));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//SpringArm->bUsePawnControlRotation = true;
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);  // 회전 속도
#pragma endregion

#pragma region	connect AnimMontage

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAttackMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/MyRobo/Animation/AM_MeleeAttack.AM_MeleeAttack'"));
	if (MeleeAttackMontageFinder.Succeeded())
	{
		MeleeAttackMontage = MeleeAttackMontageFinder.Object;
	}
#pragma endregion

	RoboComponent = CreateDefaultSubobject<URoboComponent>(TEXT("RoboComponent"));

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_LongPress.BP_LongPress_C"));
	if (InteractionWidgetClassFinder.Succeeded())
		InteractionWidgetClass = InteractionWidgetClassFinder.Class;
	InteractionWidget->SetWidgetClass(InteractionWidgetClass);

	TeamId = FGenericTeamId(0);

	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComponent"));
	if (StimuliSourceComponent)
	{
		StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimuliSourceComponent->RegisterWithPerceptionSystem();
	}
}

FGenericTeamId AMyRobo::GetGenericTeamId() const
{
	return TeamId;
}

AWeapon* AMyRobo::FindNearbyWeapon()
{
	FVector CheckLoc = GetMesh()->GetSocketLocation(TEXT("Weapon"));
	float SearchRadius = 150.f;

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);

	bool IsHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		CheckLoc,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_GameTraceChannel8),
		Sphere
	);

	if (!IsHit) return nullptr;

	AWeapon* ClosestWeapon = nullptr;
	float ClosestDistSq = MAX_FLT;

	for (auto& Hit : Overlaps)
	{
		AWeapon* Weapon = Cast<AWeapon>(Hit.GetActor());
		if (!Weapon || Weapon == CurrentWeapon) continue;

		float DistSq = FVector::DistSquared(CheckLoc, Weapon->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			ClosestWeapon = Weapon;
		}
	}

	return ClosestWeapon;
}

void AMyRobo::DropCurrentWeapon()
{
	if (!CurrentWeapon) return;

	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->FindComponentByClass<USkeletalMeshComponent>();
	if(WeaponMesh)
	{
		CurrentWeapon->SetActorEnableCollision(true);
		CurrentWeapon = nullptr;
	}

	CurrentWeapon = nullptr;
}

void AMyRobo::EquipWeapon(AWeapon* NewWeapon)
{
	if (!NewWeapon) return;

	USkeletalMeshComponent* WeaponMesh = NewWeapon->FindComponentByClass<USkeletalMeshComponent>();

	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//무기마다 크기가 다를 경우 코드를 세분화해서 크기 부분을 통제한다
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	////////여기서 무기별로 탐색해서 각 소켓에 붙여야 하나
	NewWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("Weapon"));

	CurrentWeapon = NewWeapon;

}

// Called when the game starts or when spawned
void AMyRobo::BeginPlay()
{
	Super::BeginPlay();

	InteractionWidget->SetHiddenInGame(true);
}

// Called every frame
void AMyRobo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DepthBelowSurface = GetDepthBelowSurface();

	/*if (BuoyancyComponent->GetCurrentWaterBodyComponents().IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Blue, TEXT("Out Sea"));
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Blue, TEXT("In Sea"));
	}*/


	bool bInWater = !BuoyancyComponent->GetCurrentWaterBodyComponents().IsEmpty();
	//const bool bInWater = DepthBelowSurface > 0.f;

	if (bInWater)
	{
		if (GetCharacterMovement()->MovementMode != MOVE_Swimming)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Swimming"));
		}
	}
	else
	{
		if (GetCharacterMovement()->MovementMode != MOVE_Walking)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Walking"));
		}
	}

	if (IsHolding && CurrentInteractable)
	{
		HoldElapsed += DeltaTime;
		const float Duration = FMath::Max(HoldDuration, 0.01f);
		const float Percent = FMath::Clamp(HoldElapsed / Duration, 0.f, 1.f);
		UpdateInteractionProgress(Percent);

		if (Percent >= 1.f)
		{
			IsHolding = false;
			UpdateInteractionProgress(0.f);

			if (CurrentInteractable)
			{
				CurrentInteractable->Interact();
			}

			ShowInteractionWidget(false);
		}
	}

}

// Called to bind functionality to input
void AMyRobo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyRobo::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	switch (GetCharacterMovement()->MovementMode)
	{
	case EMovementMode::MOVE_Swimming:
		GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true; 
		break;
	case EMovementMode::MOVE_Walking:
		GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
		break;
	}
}

//controller 생성이후 호출되는 함수
void AMyRobo::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	MainController = Cast<AMyCharacterController>(NewController);

	RoboComponent->InitOxygen();
}

void AMyRobo::setupMainUIReference(UMainUI* InMainUI)
{
	if (InMainUI)
	{
		//델리게이트 등록
		RoboComponent->OnHPChanged.BindLambda([this, InMainUI](float value) {
			InMainUI->SetHPPercent(value);
			});
		RoboComponent->OnDepthChanged.BindLambda([this, InMainUI](float value) {
			InMainUI->SetMeters(value);
			});
		InventoryComponent->OnInventoryChanged.BindLambda([InMainUI](const TArray<FCaughtFishInfo>& FishList) {
			InMainUI->ShowRankUI(FishList);
			});
	}
}


void AMyRobo::PlayMontageFullBody(TObjectPtr<UAnimMontage> Montage, FName SectionName)
{
	if (Montage == nullptr) return;
	BodyComponent->GetAnimInstance()->Montage_Play(Montage);

	if (SectionName.IsNone() == false)
	{
		BodyComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
	}
}

void AMyRobo::PlayMeleeAttackMontage()
{
	if (nullptr != MeleeAttackMontage)
	{
		PlayMontageFullBody(MeleeAttackMontage);
	}
		/*if (GetMovementComponent()->IsFalling() == true || nullptr == MeleeAttackMontage || isMeleeAttack == false
			|| BodyComponent->GetAnimInstance()->Montage_IsPlaying(MeleeAttackMontage))
			return;
		PlayMontageFullBody(MeleeAttackMontage);*/
		/*PlayMontageFullBody(MeleeAttackMontage, AttackSectionNames[AttackIndex]);
		++AttackIndex;
		AttackIndex %= AttackSectionNames.Num();*/
}

void AMyRobo::WeaponActive()
{
	/*isEquip = true;
	WeaponComponent->AttachToComponent(BodyComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
	FName(TEXT("Weapon")));*/
}

void AMyRobo::WeaponInactive()
{
	//	isEquip = false;
	//	WeaponComponent->AttachToComponent(BodyComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
	//		FName(TEXT("Weapon")));
}


float AMyRobo::GetDepthBelowSurface() const
{
	if (!BuoyancyComponent) return 0.0f;

	// 현재 캐릭터의 위치
	//FVector ActorLocation = BuoyancyComponent->GetOwner()->GetActorLocation();

	// BuoyancyComponent가 인식 중인 WaterBody 목록 가져오기
	const auto& WaterBodies = BuoyancyComponent->GetCurrentWaterBodyComponents();

	if (UWaterBodyComponent* Water = WaterBodies[0].Get())    // .Get()로 유효 포인터 획득
	{
		const FVector Loc = GetActorLocation();
		FVector SurfLoc, SurfNormal, SurfVelocity;
		float OutDepth = 0.f;

		Water->GetWaterSurfaceInfoAtLocation(
			Loc, SurfLoc, SurfNormal, SurfVelocity, OutDepth, /*bIncludeDepth=*/true);

		// 수면 기준 잠수 깊이(+면 수면 아래)
		return SurfLoc.Z - Loc.Z;
	}

	return 0.0f; // 물에 없음
}

void AMyRobo::AttackTrace()
{
	TArray<FHitResult> HitResult;
	bool isHit = UKismetSystemLibrary::BoxTraceMulti(
	this,
	GetActorLocation(),
	GetActorLocation() + GetActorForwardVector() * 100.0f,
	FVector(50.0f, 50.0f, 50.0f),
	FRotator::ZeroRotator,
	UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3),
	false,
	{},
	EDrawDebugTrace::ForDuration,
	HitResult,
	true 
	);
	if (isHit)
	{
		for (const FHitResult& result : HitResult)
		{
			ASeaCreature* SeaCreature = Cast<ASeaCreature>(result.GetActor());
			if (SeaCreature != nullptr)
			{
				SeaCreature->HitBy(RoboComponent->GetDamage(),result);
			}
		}
	}
}

void AMyRobo::ShowInteractionWidget(bool bShow)
{
	if (!InteractionWidget)
		return;

	if (bShow)
	{
		InteractionWidget->SetHiddenInGame(false);
		AActor* InteractionActor = Cast<AActor>(InteractionObject);
		if (InteractionActor)
			InteractionWidget->SetWorldLocation(InteractionActor->GetActorLocation());
	}
	else
	{
		InteractionWidget->SetHiddenInGame(true);
	}

	if (UUserWidget* UserWidget = InteractionWidget->GetUserWidgetObject())
	{
		if (ULongPressUI* UI = Cast<ULongPressUI>(UserWidget))
		{
			UI->SetLongPressBarPercent(0.0f);
		}
	}

}

void AMyRobo::StartSpaceHold()
{
	if (!CurrentInteractable)
		return;

	IsHolding = true;
	HoldElapsed = 0.f;
	UpdateInteractionProgress(0.f);
}

void AMyRobo::StopSpaceHold()
{
	if (HoldElapsed < HoldDuration)
	{
		HandleShortPress();
	}
	IsHolding = false;
	HoldElapsed = 0.f;
	UpdateInteractionProgress(0.f);
}

void AMyRobo::HandleShortPress()
{
	IsHolding = false;

	// 근처 무기 유무 확인
	AWeapon* ClosestWeapon = FindNearbyWeapon();
	if (!ClosestWeapon) return;

	// 무기 장착중이라면, 무기를 버린다
	if (CurrentWeapon)
	{
		DropCurrentWeapon();
	}

	//무기 장착
	EquipWeapon(ClosestWeapon);
}

void AMyRobo::UpdateInteractionProgress(float Percent)
{
	if (!InteractionWidget) return;

	if (UUserWidget* UserWidget = InteractionWidget->GetWidget())
	{
		if (ULongPressUI* LongPress = Cast<ULongPressUI>(UserWidget))
		{
			LongPress->SetLongPressBarPercent(Percent);
		}
	}
}

void AMyRobo::FocusOnInteractionTarget(IInteractionObject* Target)
{

}

void AMyRobo::FireCurrentWeaponAt(const FVector& SpawnLocation, const FVector& AimDirection)
{
	if (CurrentWeapon && CurrentWeapon->WeaponStats && CurrentWeapon->WeaponStats->Category == EWeaponCategory::Ranged)
	{
		CurrentWeapon->Attack(this, AimDirection);
	}
}

#pragma region reference

//void AMyCharacter::HitBy(float DamageAmount)
//{
//	StateComponent->TakeDamage(DamageAmount);
//}
//
#pragma endregion
