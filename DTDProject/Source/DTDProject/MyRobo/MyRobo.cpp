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
#include "UI/MainUI.h"
#include "UI/LongPressUI.h"
#include "ActorComponent/StateComponent/RoboComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "BuoyancyComponent.h"
#include "WaterBodyComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SeaCreature/SeaCreature.h"
#include "Engine/OverlapResult.h"
#include "Interface/InteractionObject.h"
#include "Weapon/Weapon.h"
#include "Object/RandomBox.h"
#include "Animation/AnimInstance.h"


AMyRobo::AMyRobo()
{
	PrimaryActorTick.bCanEverTick = true;
	WasOnSurface = false;

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

#pragma region	Animation
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactionMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/MyRobo/Animation/AM_HitBy.AM_HitBy'"));
	if (HitReactionMontageFinder.Succeeded())
	{
		HitReactionMontage = HitReactionMontageFinder.Object;
	}

#pragma endregion

	RoboComponent = CreateDefaultSubobject<URoboComponent>(TEXT("RoboComponent"));

#pragma region Widgets
	LongPressWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LongPressWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> LongPressWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_LongPress.BP_LongPress_C"));
	if (LongPressWidgetClassFinder.Succeeded())
		LongPressWidgetClass = LongPressWidgetClassFinder.Class;
	LongPressWidget->SetWidgetClass(LongPressWidgetClass);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> PickupWidgetClassFinder(TEXT("/Game/BluePrint/UI/BP_Pickup.BP_Pickup_C"));
	if (PickupWidgetClassFinder.Succeeded())
	{
		PickupWidgetClass = PickupWidgetClassFinder.Class;
	}
	PickupWidget->SetWidgetClass(PickupWidgetClass);
#pragma endregion

#pragma region AI
	TeamId = FGenericTeamId(0);

	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComponent"));
	if (StimuliSourceComponent)
	{
		StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimuliSourceComponent->RegisterWithPerceptionSystem();
	}
#pragma endregion
}

FGenericTeamId AMyRobo::GetGenericTeamId() const
{
	return TeamId;
}

void AMyRobo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BuoyancyComponent)
	{
		const float CurrentDepth = GetDepthBelowSurface();

		if (RoboComponent)
		{
			RoboComponent->UpdateCurrentDepth(CurrentDepth);
		}

		const bool IsOnSurface = (CurrentDepth <= 0.0f);
		if (IsOnSurface && !WasOnSurface)
		{
			OnSurfaced.Broadcast();
		}
		WasOnSurface = IsOnSurface;
	}


	bool bInWater = !BuoyancyComponent->GetCurrentWaterBodyComponents().IsEmpty();

	if (bInWater)
	{
		if (GetCharacterMovement()->MovementMode != MOVE_Swimming)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
		}
	}
	else
	{
		if (GetCharacterMovement()->MovementMode != MOVE_Walking)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
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

			ShowLongPressWidget(false, nullptr);

			if (bIsCameraFixed)
			{
				FocusOnInteractionTarget(nullptr);
			}
		}
	}

	if (MainController)
	{
		if (APlayerCameraManager* CameraManager = MainController->PlayerCameraManager)
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

void AMyRobo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyRobo::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PreviousCustomMode == MOVE_Swimming && GetCharacterMovement()->MovementMode != MOVE_Swimming)
	{
		if (!BuoyancyComponent->GetCurrentWaterBodyComponents().IsEmpty())
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
			return;
		}
	}

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

void AMyRobo::SetupMainUIReference(UMainUI* InMainUI)
{
	if (InMainUI)
	{
		if (RoboComponent)
		{
			RoboComponent->OnHPChanged.BindLambda([this, InMainUI](float value) {
				InMainUI->SetHPPercent(value);
				});
			RoboComponent->OnDepthChanged.BindLambda([this, InMainUI](float value) {
				InMainUI->SetMeters(value);
				});
			RoboComponent->OnOxygenDepleted.AddLambda([InMainUI]
				{
					InMainUI->ShowGameResultUI(false);
				});
		}
		if (InventoryComponent)
		{
			InventoryComponent->OnFishCollected.AddUObject(InMainUI, &UMainUI::ShowCollectedFishNotification);
			InventoryComponent->OnWeightChanged.AddLambda([InMainUI](float current, float max) {
				InMainUI->SetWeights(current, max);
				});
			InventoryComponent->OnBecameOverweight.AddLambda([InMainUI](bool becameOverweight) {
				InMainUI->OverWeightNotification(becameOverweight);
				});
			InventoryComponent->OnToolSlotUpdated.AddUObject(InMainUI, &UMainUI::OnUpdateToolSlot);
			InventoryComponent->OnActiveToolChanged.AddUObject(InMainUI, &UMainUI::OnChangeActiveTool);

			//최종 결과 보여줄 때
		/*InventoryComponent->OnInventoryChanged.BindLambda([InMainUI](const TArray<FCaughtFishInfo>& FishList) {
			InMainUI->ShowCollectedFishNotification(FishList,3.0f);
			});*/
		}

		OnSurfaced.AddLambda([this]()
			{
				if (MainController)
				{
					MainController->EndMyGame(true);
				}
			});

		OnWeaponSlotUpdated.AddUObject(InMainUI, &UMainUI::UpdateWeaponSlot);

		BroadcastCurrentWeaponStates();

		if (GunWeapon)
		{
			GunWeapon->OnGunAmmoUpdated.AddDynamic(InMainUI, &UMainUI::UpdateGunAmmo);
		}
	}
}

void AMyRobo::BeginRangedAim()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HolsterTimerHandle);
	}
	CurrentWeaponState = EWeaponState::RangedAttaching;
	UpdateWeaponAttachments();
	PlayRangedAimMontage();
}

void AMyRobo::EndRangedAim()
{
	PlayRangedStopAimMontage();
}

void AMyRobo::StartFiring(const FVector& FireDirection)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HolsterTimerHandle);
	}

	bIsFiring = true;

	if (ActiveRangedWeapon)
	{
		ActiveRangedWeapon->StartFire(this, FireDirection);
	}
}

void AMyRobo::StopFiring()
{
	bIsFiring = false;

	if (ActiveRangedWeapon)
	{
		ActiveRangedWeapon->StopFire(this);
	}
}

void AMyRobo::PerformMeleeAttack()
{
	if (!MainController) return;

	GetWorld()->GetTimerManager().ClearTimer(HolsterTimerHandle);

	if (MeleeWeapon)
	{
		CurrentWeaponState = EWeaponState::MeleeAttaching;
		UpdateWeaponAttachments();
		PlayMeleeAttackMontage();
	}
}

void AMyRobo::PerformAttack(const FVector2D& ScreenPosition)
{
	if (!MainController) return;

	GetWorld()->GetTimerManager().ClearTimer(HolsterTimerHandle);

	if (ActiveRangedWeapon)
	{
		RangedTargetScreenPosition = ScreenPosition;
		StopAnimMontage();
		PlayRangedAttackMontage();
	}
}
// AnimNotify에서 호출될 실제 발사 함수
void AMyRobo::FireProjectile()
{
	if (!ActiveRangedWeapon || !MainController) return;

	// 1. 플레이어의 '진짜 조준 방향'void AMyRobo::FireProjectile()을 컨트롤러로부터 직접 가져옵니다.
	//    이것이 카메라의 각도와 무관한, 플레이어의 순수한 의도입니다.
	const FRotator ControlRotation = MainController->GetControlRotation();
	const FVector ControlDirection = ControlRotation.Vector();

	// 2. 광선의 시작점은 '카메라 위치'로 설정하여, 플레이어의 시야에서 장애물에 가려지는지를 확인합니다.
	FVector TraceStart, ForwardVector;
	MainController->DeprojectAimToWorld(TraceStart, ForwardVector);

	// 3. 광선의 끝점은 카메라 위치에서 '컨트롤러의 조준 방향'으로 길게 뻗어나갑니다.
	const FVector TraceEnd = TraceStart + (ForwardVector * 10000.f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(ActiveRangedWeapon);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	/*DrawDebugSphere(GetWorld(), TraceStart, 25.f, 12, FColor::Emerald, false, 2.0f);
	DrawDebugDirectionalArrow(GetWorld(), TraceStart, TraceEnd, 50.f, FColor::Red, false, 10.f);*/

	FVector FinalTargetLocation;
	if (HitResult.bBlockingHit)
	{
		FinalTargetLocation = HitResult.Location;
	}
	else
	{
		FinalTargetLocation = TraceEnd;
	}

	DrawDebugSphere(GetWorld(), FinalTargetLocation, 25.f, 12, FColor::Red, false, 2.0f);
	//DrawDebugLine(GetWorld(), TraceStart, FinalTargetLocation, FColor::Blue, false, 2.0f, 0, 1.f);

	const FVector MuzzleLocation = ActiveRangedWeapon->GetMuzzleLocation();
	// 최종 발사 방향은 '총구 위치'에서 '최종 목표 지점'을 향하는 방향입니다.
	FVector FireDirection = (FinalTargetLocation - MuzzleLocation).GetSafeNormal();

	//DrawDebugLine(GetWorld(), MuzzleLocation, MuzzleLocation + FireDirection * 10000.f, FColor::Green, false, 2.0f, 0, 1.f);

	ActiveRangedWeapon->SpawnProjectileAtMuzzle(FireDirection);
}
//원거리 무기 교체
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
	UpdateWeaponAttachments();
}

AWeapon* AMyRobo::GetActiveWeapon() const
{
	if (CurrentWeaponState == EWeaponState::MeleeAttaching)
	{
		return MeleeWeapon;
	}
	if (CurrentWeaponState == EWeaponState::RangedAttaching)
	{
		return ActiveRangedWeapon;
	}

	return nullptr;
}

void AMyRobo::ShowLongPressWidget(bool bShow, AActor* TargetActor)
{
	if (!LongPressWidget) return;

	if (bShow && TargetActor)
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
		FVector TargetLocation = TargetActor->GetActorLocation();
		FVector WidgetLocation = TargetLocation + FVector(0.0f, 0.0f, 20.0f);
		PickupWidget->SetWorldLocation(WidgetLocation);
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
	if (AcquirableWeapon)
	{
		ShowPickupWidget(false, AcquirableWeapon);
	}

	AcquirableWeapon = Weapon;

	if (AcquirableWeapon)
	{
		ShowPickupWidget(true, AcquirableWeapon);
		UE_LOG(LogTemp, Log, TEXT("[MyRobo] Acquirable weapon set: %s"), *Weapon->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[MyRobo] Acquirable weapon cleared."));
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
				MainController->SetIgnoreLookInput(true); //마우스 정지

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
			MainController->SetIgnoreLookInput(false); //마우스 정지해제
		}
	}
}

//게임 시작 시 무기 UI 초기화
void AMyRobo::BroadcastCurrentWeaponStates()
{
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
}

void AMyRobo::AttackTrace()
{
	if (!MeleeWeapon || !MeleeWeapon->GetWeaponStats()) return;

	const float WeaponDamage = MeleeWeapon->GetWeaponStats()->Damage;

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
				SeaCreature->HitBy(WeaponDamage, result);
			}
		}
	}
}

void AMyRobo::CollectSeaCreature(ASeaCreature* FishToCollect)
{
	if (!FishToCollect || !FishToCollect->GetData()) return;

	if(InventoryComponent)
	{
		FCaughtFishInfo Info;
		const FSeaCreatureData* FishData = FishToCollect->GetData();

		Info.FishName = FishData->Name;
		Info.Weight = FishData->Weight;
		Info.Grade = FishData->Grade;
		Info.FishIcon = FishData->FishIcon;
		InventoryComponent->AddCaughtFish(Info);
	}
	FishToCollect->SetLifeSpan(0.1f);
}

void AMyRobo::HitBy(AActor* DamageCauser, const FHitResult& HitResult)
{
	if (!RoboComponent || !DamageCauser) return;

	ASeaCreature* Fish = Cast<ASeaCreature>(DamageCauser);

	if (Fish && Fish->GetData())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Robo HitBy Call : %s"), *Fish->GetName()));

		const float DamageAmount = Fish->GetData()->Damage;

		RoboComponent->TakeDamage(DamageAmount, HitResult);

		if (!RoboComponent->IsDead())
		{
			if (HitReactionMontage && BodyComponent && BodyComponent->GetAnimInstance())
			{
				BodyComponent->GetAnimInstance()->Montage_Play(HitReactionMontage);
			}
		}
	}
}

void AMyRobo::DieRobo()
{
	if (InputEnabled() == false) return;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (MainController)
	{
		DisableInput(MainController);

		MainController->EndMyGame(false);
	}

	// UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// if (AnimInstance && DeathMontage)
	// {
	//	 AnimInstance->Montage_Play(DeathMontage);
	// }
}

//컴포넌트 초기화 이후 호출, 컨트롤러가 널일 확률 높음
void AMyRobo::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//MainController = Cast<AMyCharacterController>(GetController());
	//if (!MainController) return;

	//if (UMainUI* MainUI = MainController->GetMainUI())
	//{
	//	SetupMainUIReference(MainUI);
	//}
}

//빙의할 떄 호출, 컨트롤러가 유효하다는 것이 보장되는 첫 번째 시점
void AMyRobo::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (RoboComponent)
	{
		RoboComponent->InitOxygen();
	}
}

void AMyRobo::BeginPlay()
{
	Super::BeginPlay();

	LongPressWidget->SetHiddenInGame(true);
	PickupWidget->SetHiddenInGame(true);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	USkeletalMeshComponent* CharacterMesh = GetMesh();

	if (DefaultMeleeWeaponClass)
	{
		MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultMeleeWeaponClass, SpawnParams);
		if(MeleeWeapon)
		{
			MeleeWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::
				SnapToTargetNotIncludingScale, 
				MeleeWeaponSocketName);
			MeleeWeapon->RowName = TEXT("Melee");
			MeleeWeapon->PostInitializeComponents();
			MeleeWeapon->SetActorHiddenInGame(true);
		}
	}

	if (DefaultHarpoonWeaponClass)
	{
		HarpoonWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultHarpoonWeaponClass, SpawnParams);
		if (HarpoonWeapon)
		{
			HarpoonWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, RangedWeaponSocketName);
			HarpoonWeapon->RowName = TEXT("Harpoon");
			HarpoonWeapon->PostInitializeComponents();
			HarpoonWeapon->SetActorHiddenInGame(true);
		}
	}

	if (DefaultGunWeaponClass)
	{
		GunWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultGunWeaponClass, SpawnParams);
		if (GunWeapon)
		{
			GunWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, RangedWeaponSocketName);
			GunWeapon->RowName = TEXT("Gun");
			GunWeapon->PostInitializeComponents();
			GunWeapon->SetActorHiddenInGame(true);
		}
	}

	ActiveRangedWeapon = HarpoonWeapon;
	CurrentWeaponState = EWeaponState::Unarmed;


	MainController = Cast<AMyCharacterController>(GetController());
	if (!MainController) return;

	if (UMainUI* MainUI = MainController->GetMainUI())
	{
		SetupMainUIReference(MainUI);
	}
}

void AMyRobo::PlayMontageFullBody(TObjectPtr<UAnimMontage> Montage, FOnMontageEnded& EndDelegate, FName SectionName, float PlayRate)
{
	if (Montage == nullptr) return;

	if (UAnimInstance* AnimInstance = BodyComponent->GetAnimInstance())
	{
		if (PlayRate<0.0f || !AnimInstance->Montage_IsPlaying(Montage))
		{
			const float Duration = AnimInstance->Montage_Play(Montage,PlayRate);

			if(Duration>0.f)
			{
				if (EndDelegate.IsBound())
				{
					AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
				}

				if (SectionName.IsNone() == false)
				{
					AnimInstance->Montage_JumpToSection(SectionName, Montage);
				}
			}
		}
	}
}

void AMyRobo::PlayMeleeAttackMontage()
{
	if (MeleeWeapon && MeleeWeapon->GetWeaponStats() && MeleeWeapon->GetWeaponStats()->AttackMontage)
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMyRobo::OnAttackMontageEnded);
		PlayMontageFullBody(MeleeWeapon->GetWeaponStats()->AttackMontage, EndDelegate);
	}
}

void AMyRobo::PlayRangedAimMontage()
{
	if (ActiveRangedWeapon && ActiveRangedWeapon->GetWeaponStats() && ActiveRangedWeapon->GetWeaponStats()->AimMontage)
	{
		FOnMontageEnded EndDelegate;
		PlayMontageFullBody(ActiveRangedWeapon->GetWeaponStats()->AimMontage, EndDelegate, FName("Default"));
	}
}

void AMyRobo::PlayRangedStopAimMontage()
{
	if (ActiveRangedWeapon && ActiveRangedWeapon->GetWeaponStats() && ActiveRangedWeapon->GetWeaponStats()->AimMontage)
	{
		FOnMontageEnded EndDelegate;
		//EndDelegate.BindUObject(this, &AMyRobo::OnStopAimMontageEnded);
		PlayMontageFullBody(ActiveRangedWeapon->GetWeaponStats()->AimMontage, EndDelegate, NAME_None, -1.0f); //NAME_None : 특정 세션으로 점프하지말고, 그냥 처음부터 재생해라.
	}
}

void AMyRobo::PlayRangedAttackMontage()
{
	if (ActiveRangedWeapon && ActiveRangedWeapon->GetWeaponStats() && ActiveRangedWeapon->GetWeaponStats()->AttackMontage )
	{
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMyRobo::OnAttackMontageEnded);
		PlayMontageFullBody(ActiveRangedWeapon->GetWeaponStats()->AttackMontage, EndDelegate);
	}

}

void AMyRobo::StopAnimMontage(float BlendOutTime)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(BlendOutTime);
	}
}

void AMyRobo::HandleShortPress()
{
	IsHolding = false;

	if (AcquirableWeapon)
	{
		UE_LOG(LogTemp, Log, TEXT("[MyRobo] Short press detected. Attempting to pick up %s."), *AcquirableWeapon->GetName());
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

void AMyRobo::PickupAcquirableWeapon()
{
	if (!AcquirableWeapon) return;

	ARandomBox* BoxOwner = Cast<ARandomBox>(AcquirableWeapon->GetOwner());

	if (BoxOwner)
	{
		AcquirableWeapon->ReloadToMax(); //첫 생성에만 풀충전
	}

	FVector DropLocation = AcquirableWeapon->GetActorLocation();

	EWeaponSlot SlotToFill = AcquirableWeapon->GetSlotType();
	AWeapon* OldWeapon = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[MyRobo] PickupAcquirableWeapon started for slot: %s"), *UEnum::GetValueAsString(SlotToFill));

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

	if (SlotToFill == EWeaponSlot::Gun && MainController)
	{
		if (UMainUI* MainUI = MainController->GetMainUI())
		{
			if (OldWeapon)
			{
				OldWeapon->OnGunAmmoUpdated.RemoveAll(MainUI);
			}
			if (GunWeapon)
			{
				GunWeapon->OnGunAmmoUpdated.AddDynamic(MainUI, &UMainUI::UpdateGunAmmo);
			}
		}
	}

	AcquirableWeapon->SetOwner(this);
	AcquirableWeapon->SetActorHiddenInGame(true);

	//UI에 특정 슬롯이 업데이트되었음을 알림
	OnWeaponSlotUpdated.Broadcast(SlotToFill, AcquirableWeapon);

	if (OldWeapon == ActiveRangedWeapon)
	{
		ActiveRangedWeapon = AcquirableWeapon;
	}

	if (OldWeapon && BoxOwner)
	{
		OldWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		OldWeapon->SetActorLocation(DropLocation);
		OldWeapon->SetActorHiddenInGame(false);
		BoxOwner->SetSpawnedWeapon(OldWeapon);
	}
	else if (BoxOwner)
	{
		BoxOwner->ClearSpawnedWeapon();
	}

	SetAcquirableWeapon(nullptr);
}

void AMyRobo::UpdateWeaponAttachments()
{
	if (MeleeWeapon) MeleeWeapon->SetActorHiddenInGame(true);
	if (HarpoonWeapon) HarpoonWeapon->SetActorHiddenInGame(true);
	if (GunWeapon) GunWeapon->SetActorHiddenInGame(true);

	switch (CurrentWeaponState)
	{
	case EWeaponState::Unarmed:
		break;

	case EWeaponState::MeleeAttaching:
		if (MeleeWeapon)
		{
			MeleeWeapon->AttachToComponent(GetMesh(),	FAttachmentTransformRules::SnapToTargetNotIncludingScale, MeleeWeaponSocketName);
			MeleeWeapon->SetActorHiddenInGame(false);
		}
		break;

	case EWeaponState::RangedAttaching:
		if (ActiveRangedWeapon)
		{
			if(!RangedWeaponSocketName.IsNone())
			{
				ActiveRangedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RangedWeaponSocketName);
				ActiveRangedWeapon->SetActorHiddenInGame(false);
			}
		}
		break;
	}
}

void AMyRobo::StartHolsterTimer()
{
	GetWorld()->GetTimerManager().SetTimer(HolsterTimerHandle, this, &AMyRobo::HolsterWeapons, 3.0f, false);
}

void AMyRobo::HolsterWeapons()
{
	CurrentWeaponState = EWeaponState::Unarmed;
	UpdateWeaponAttachments();
}

void AMyRobo::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (MainController)
	{
		MainController->SetIsAttacking(false);
	}

	if (!bIsFiring)
	{
		StartHolsterTimer();
	}
}

void AMyRobo::OnStopAimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bIsFiring)
	{
		StartHolsterTimer();
	}
}

float AMyRobo::GetDepthBelowSurface() const
{
	if (!BuoyancyComponent) return 0.0f;
	// BuoyancyComponent가 인식 중인 WaterBody 목록 가져오기
	const TArray<UWaterBodyComponent*>& WaterBodies = BuoyancyComponent->GetCurrentWaterBodyComponents();
	// 물 속에 있는지, 그리고 WaterBodies 배열이 비어있지 않은지 확인합니다.
	if (WaterBodies.Num() > 0 && WaterBodies[0] != nullptr)
	{	// 첫 번째 WaterBody 컴포넌트를 가져옵니다.
		UWaterBodyComponent* Water = WaterBodies[0];
		const FVector Loc = GetActorLocation();

		FVector SurfLoc,SurfNormal, SurfVelocity;
		float OutDepth = 0.f;
		// GetWaterSurfaceInfoAtLocation 함수는 수면 위치(SurfLoc)를 반환합니다.
		Water->GetWaterSurfaceInfoAtLocation(Loc, SurfLoc, SurfNormal, SurfVelocity, OutDepth, true);

		const float Depth = SurfLoc.Z - Loc.Z;

		// 이 함수는 위치가 수면 위일 경우 음수 값을 반환할 수 있습니다.
		// 따라서 Max(0.f, ...)를 사용하여 항상 0 이상의 값만 반환하도록 합니다.
		return FMath::Max(0.f, Depth / 100.0f); // cm 단위를 m 단위로 변경
	}
	return 0.0f; // 물 밖에 있거나 수면 위에 있으면 0을 반환
}