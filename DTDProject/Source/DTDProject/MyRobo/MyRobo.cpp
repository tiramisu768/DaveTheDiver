// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRobo/MyRobo.h"
#include "Controller/MyCharacterController/MyCharacterController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
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
#include "Kismet/KismetMathLibrary.h"
#include "SeaCreature/SeaCreature.h"
#include "Engine/OverlapResult.h"
#include "Interface/InteractionObject.h"
#include "Weapon/Weapon.h"
#include "Object/RandomBox.h"


AMyRobo::AMyRobo()
{
	PrimaryActorTick.bCanEverTick = true;
#pragma region Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	BodyComponent = GetMesh();
	BodyComponent->SetupAttachment(GetRootComponent());

	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

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

	LongPressWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LongPressWidget"));
	//LongPressWidget->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> LongPressWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_LongPress.BP_LongPress_C"));
	if (LongPressWidgetClassFinder.Succeeded())
		LongPressWidgetClass = LongPressWidgetClassFinder.Class;
	LongPressWidget->SetWidgetClass(LongPressWidgetClass);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	//PickupWidget->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> PickupWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Pickup.BP_Pickup_C"));
	if (PickupWidgetClassFinder.Succeeded())
	{
		PickupWidgetClass = PickupWidgetClassFinder.Class;
	}
	PickupWidget->SetWidgetClass(PickupWidgetClass);

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

// Called when the game starts or when spawned
void AMyRobo::BeginPlay()
{
	Super::BeginPlay();

	LongPressWidget->SetHiddenInGame(true);
	PickupWidget->SetHiddenInGame(true);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	if (DefaultMeleeWeaponClass)
	{
		MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultMeleeWeaponClass, SpawnParams);
		MeleeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));
		MeleeWeapon->SetActorHiddenInGame(true);
	}

	if (DefaultHarpoonWeaponClass)
	{
		HarpoonWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultHarpoonWeaponClass, SpawnParams);
		HarpoonWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));
		HarpoonWeapon->SetActorHiddenInGame(true);
	}

	if (DefaultGunWeaponClass)
	{
		GunWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultGunWeaponClass, SpawnParams);
		GunWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));
		GunWeapon->SetActorHiddenInGame(true);
	}

	if (MeleeWeapon)
	{
		// Melee 슬롯이 업데이트되었음을 알림
		OnWeaponSlotUpdated.Broadcast(EWeaponSlot::Melee, MeleeWeapon);
	}
	if (HarpoonWeapon)
	{
		// Harpoon 슬롯이 업데이트되었음을 알림
		OnWeaponSlotUpdated.Broadcast(EWeaponSlot::Harpoon, HarpoonWeapon);
	}
	if (GunWeapon)
	{
		// Gun 슬롯이 업데이트되었음을 알림
		OnWeaponSlotUpdated.Broadcast(EWeaponSlot::Gun, GunWeapon);
	}

	ActiveRangedWeapon = MeleeWeapon;
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
				CurrentInteractable->Interact(this);
			}

			ShowLongPressWidget(false,nullptr);

			if (bIsCameraFixed)
			{
				FocusOnInteractionTarget(nullptr);
			}
		}
	}

	if (MainController)
	{
		APlayerCameraManager* CameraManager = MainController->PlayerCameraManager;
		if (CameraManager)
		{
			FVector CameraLocation = CameraManager->GetCameraLocation();
			if (LongPressWidget && LongPressWidget->IsVisible())
			{
				FVector WidgetLocation = LongPressWidget->GetComponentLocation();
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
				LongPressWidget->SetWorldRotation(LookAtRotation);
			}

			if (PickupWidget && PickupWidget->IsVisible())
			{
				FVector WidgetLocation = PickupWidget->GetComponentLocation();
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
				PickupWidget->SetWorldRotation(LookAtRotation);
			}
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

		OnWeaponSlotUpdated.AddDynamic(InMainUI, &UMainUI::OnUpdateWeaponSlot);
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
	if (MeleeWeapon && MeleeWeapon->WeaponStats && MeleeWeapon->WeaponStats->AttackMontage)
	{
		PlayMontageFullBody(MeleeWeapon->WeaponStats->AttackMontage);
	}
}

void AMyRobo::PlayRangedAttackMontage()
{
	if (ActiveRangedWeapon && ActiveRangedWeapon->WeaponStats && ActiveRangedWeapon->WeaponStats->AttackMontage)
	{
		PlayMontageFullBody(ActiveRangedWeapon->WeaponStats->AttackMontage);
	}
}

void AMyRobo::PerformAttack()
{
	if (!MainController) return;

	if (MainController->GetIsAiming())
	{
		if (ActiveRangedWeapon)
		{
			PlayRangedAttackMontage();
			ActiveRangedWeapon->Attack(this);
		}
	}
	else
	{
		if (MeleeWeapon)
		{
			PlayMeleeAttackMontage();
			MeleeWeapon->Attack(this);
		}
	}
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

void AMyRobo::SwitchActiveRangedWeapon()
{
	if (ActiveRangedWeapon == HarpoonWeapon)
	{
		ActiveRangedWeapon = GunWeapon;
	}
	else
	{
		ActiveRangedWeapon = HarpoonWeapon;
	}
}

void AMyRobo::ShowLongPressWidget(bool bShow, AActor* TargetActor)
{
	if (!LongPressWidget)
		return;

	if (bShow&&TargetActor)
	{
		UpdateInteractionProgress(0.f);
		FVector TargetLocation = TargetActor->GetActorLocation();
		FVector WidgetLocation = TargetLocation + FVector(0.0f, 0.0f, 100.0f);
		LongPressWidget->SetWorldLocation(WidgetLocation);
		LongPressWidget->SetHiddenInGame(false);			
	}
	else
	{
		LongPressWidget->SetHiddenInGame(true);
	}
}

void AMyRobo::ShowPickupWidget(bool bShow, AActor* TargetActor)
{
	if (!PickupWidget) return;

	if (bShow && TargetActor)
	{
		PickupWidget->SetWorldLocation(TargetActor->GetActorLocation());
		PickupWidget->SetHiddenInGame(false);
	}
	else
	{
		PickupWidget->SetHiddenInGame(true);
	}
}

void AMyRobo::StartSpaceHold()
{
	//상호작용 대상 아무것도 없음
	if (!AcquirableWeapon && !CurrentInteractable)
		return;

	//길게 누르는 상호작용일 때 카메라 고정한다
	if (!AcquirableWeapon && CurrentInteractable)
	{
		FocusOnInteractionTarget(CurrentInteractable.GetInterface());
	}

	IsHolding = true;
	HoldElapsed = 0.f;
	UpdateInteractionProgress(0.f);
}

void AMyRobo::StopSpaceHold()
{
	//카메라고정 해제
	if (bIsCameraFixed)
	{
		FocusOnInteractionTarget(nullptr);
	}

	if (HoldElapsed < HoldDuration)
	{
		HandleShortPress();
	}
	IsHolding = false;
	HoldElapsed = 0.f;
	UpdateInteractionProgress(0.f);
}

void AMyRobo::SetAcquirableWeapon(AWeapon* Weapon)
{
	AcquirableWeapon = Weapon;
}

void AMyRobo::PickupAcquirableWeapon()
{
	if (!AcquirableWeapon) return;

	EWeaponSlot SlotToFill = AcquirableWeapon->GetSlotType();
	AWeapon* OldWeapon = nullptr;

	switch (SlotToFill)
	{
	case EWeaponSlot::Melee:
		OldWeapon = MeleeWeapon;
		MeleeWeapon = AcquirableWeapon;
		break;
	case EWeaponSlot::Harpoon:
		OldWeapon = HarpoonWeapon;
		HarpoonWeapon = AcquirableWeapon;
		break;
	case EWeaponSlot::Gun:
		OldWeapon = GunWeapon;
		GunWeapon = AcquirableWeapon;
		break;
	}

	AcquirableWeapon->SetOwner(this);
	AcquirableWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));

	if (UStaticMeshComponent* WeaponMesh = AcquirableWeapon->FindComponentByClass<UStaticMeshComponent>())
	{
		if (UStaticMesh* MeshAsset = WeaponMesh->GetStaticMesh())
		{
			FVector OriginalSize = MeshAsset->GetBounds().GetBox().GetSize();

			float MaxOriginalSize = FMath::Max3(OriginalSize.X, OriginalSize.Y, OriginalSize.Z);
			if (MaxOriginalSize > KINDA_SMALL_NUMBER)
			{
				float ScaleMultiplier = TargetWeaponSize / MaxOriginalSize;
				AcquirableWeapon->SetActorRelativeScale3D(FVector(ScaleMultiplier));
			}
		}
	}

	AcquirableWeapon->SetActorHiddenInGame(true);

	//UI에 특정 슬롯이 업데이트되었음을 알림
	OnWeaponSlotUpdated.Broadcast(SlotToFill, AcquirableWeapon);

	if (OldWeapon == ActiveRangedWeapon)
	{
		ActiveRangedWeapon = AcquirableWeapon;
	}

	if (OldWeapon)
	{
		OldWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//랜덤상자 위로 이동시키는 로직 추가해야함
	}

	AcquirableWeapon = nullptr;
}

void AMyRobo::HandleShortPress()
{
	IsHolding = false;

	if(AcquirableWeapon)
	{
		PickupAcquirableWeapon();
		return;
	}
}

void AMyRobo::UpdateInteractionProgress(float Percent)
{
	if (!LongPressWidget) return;

	if (UUserWidget* UserWidget = LongPressWidget->GetUserWidgetObject())
	{
		if (ULongPressUI* LongPress = Cast<ULongPressUI>(UserWidget))
		{
			LongPress->SetLongPressBarPercent(Percent);
		}
	}
}

void AMyRobo::FocusOnInteractionTarget(IInteractionObject* Target)
{
	if (MainController)
	{
		if (Target)
		{
			AActor* TargetActor = Cast<AActor>(Target);
			if (TargetActor)
			{
				DisableInput(MainController);
				bIsCameraFixed = true;

				MainController->SetIgnoreLookInput(true);

				FVector StartLocation = Camera->GetComponentLocation();
				FVector TargetLocation = TargetActor->GetActorLocation();
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
				MainController->SetControlRotation(LookAtRotation);
			}
		}
		else
		{
			EnableInput(MainController);
			bIsCameraFixed = false;
			MainController->SetIgnoreLookInput(false);
		}
	}
}

#pragma region reference

//void AMyCharacter::HitBy(float DamageAmount)
//{
//	StateComponent->TakeDamage(DamageAmount);
//}
//
#pragma endregion
