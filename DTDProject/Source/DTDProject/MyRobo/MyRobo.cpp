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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAttackMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/MyRobo/Animation/AM_MeleeAttack.AM_MeleeAttack'"));
	if (MeleeAttackMontageFinder.Succeeded())
	{
		MeleeAttackMontage = MeleeAttackMontageFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Melee2AttackMontageFinder(TEXT(""));
	if (Melee2AttackMontageFinder.Succeeded())
	{
		Melee2AttackMontage = Melee2AttackMontageFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Melee3AttackMontageFinder(TEXT(""));
	if (Melee3AttackMontageFinder.Succeeded())
	{
		Melee3AttackMontage = Melee3AttackMontageFinder.Object;
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
		BroadcastCurrentWeaponStates();
	}
}

void AMyRobo::StartRangedAim()
{
	CurrentWeaponState = EWeaponState::RangedAttaching;
	UpdateWeaponAttachments();
	PlayRangedAimMontage();
}

void AMyRobo::StopRangedAim()
{
	PlayRangedStopAimMontage();
}

void AMyRobo::PerformAttack()
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

	FVector WorldLocation, WorldDirection;
	bool bSuccess = UGameplayStatics::DeprojectScreenToWorld(MainController, RangedTargetScreenPosition, WorldLocation, WorldDirection);

	if (bSuccess)
	{
		FVector TraceStart = MainController->PlayerCameraManager->GetCameraLocation();
		FVector TraceEnd = TraceStart + (WorldDirection * 10000.f);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(ActiveRangedWeapon);

		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

		FVector FinalTargetLocation = HitResult.bBlockingHit ? HitResult.Location : TraceEnd;

		const FVector MuzzleLocation = ActiveRangedWeapon->GetMuzzleLocation();
		FVector FireDirection = (FinalTargetLocation - MuzzleLocation).GetSafeNormal();

		DrawDebugLine(
		GetWorld(),
			MuzzleLocation,
			MuzzleLocation+FireDirection*5000.f,
			FColor::Green,
			false,
			2.0f,
			0,
			1.f
		);

		ActiveRangedWeapon->Attack(this, FireDirection);
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
				SeaCreature->HitBy(RoboComponent->GetDamage(), result);
			}
		}
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
			MeleeWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MeleeWeaponSocketName);
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
		UAnimMontage* MeleeMontage;
		if (MeleeWeapon->GetWeaponStats()->AttackMontage == MeleeAttackMontage)
		{
			MeleeMontage = MeleeAttackMontage;
		}
		else if (MeleeWeapon->GetWeaponStats()->AttackMontage == Melee2AttackMontage)
		{
			MeleeMontage = Melee2AttackMontage;
		}
		else if (MeleeWeapon->GetWeaponStats()->AttackMontage == Melee3AttackMontage)
		{
			MeleeMontage = Melee3AttackMontage;
		}

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMyRobo::OnAttackMontageEnded);
		PlayMontageFullBody(MeleeMontage, EndDelegate);
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

	// 만약 '무기 내리기' 몽타주가 없다면, 바로 무기를 집어넣는 타이머를 시작
	StartHolsterTimer();
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

	AcquirableWeapon->SetOwner(this);
	AcquirableWeapon->SetActorHiddenInGame(true);

	UE_LOG(LogTemp, Log, TEXT("[MyRobo] Broadcasting OnWeaponSlotUpdated for slot %s with new weapon %s."), *UEnum::GetValueAsString(SlotToFill), *AcquirableWeapon->GetName());
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
			MeleeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MeleeWeaponSocketName);
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

	if (bInterrupted)
	{
		StartHolsterTimer();
	}
}

void AMyRobo::OnStopAimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		StartHolsterTimer();
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

#pragma region reference

//void AMyCharacter::HitBy(float DamageAmount)
//{
//	StateComponent->TakeDamage(DamageAmount);
//}
//
#pragma endregion
