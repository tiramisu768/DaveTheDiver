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

void AMyRobo::AddWeaponToInventory(AWeapon* WeaponToAdd)
{
	if (!WeaponToAdd) return;

	EWeaponType Type = WeaponToAdd->GetWeaponType();

	FWeaponTypeInventory* Inventory = GetInventoryForType(Type);

	if (!Inventory)
	{
		WeaponInventory.Emplace(FWeaponTypeInventory{ Type });
		Inventory = &WeaponInventory.Last();
	}

	if (Inventory->Weapons.Num() >= 2)
	{
		AWeapon* WeaponToDrop = Inventory->Weapons[0];
		Inventory->Weapons.RemoveAt(0);
		if (WeaponToDrop)
		{
			WeaponToDrop->Destroy();
		}
	}

	Inventory->Weapons.Add(WeaponToAdd);
	WeaponToAdd->SetOwner(this);
	WeaponToAdd->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));
	WeaponToAdd->SetActorHiddenInGame(true);
}

void AMyRobo::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeapon = WeaponToEquip;

	if (CurrentWeapon)
	{
		// 새로 장착한 무기는 보이게 처리(또는 공격 / 조준 시에만 보이게 할 수도 있음)
			// 예시: CurrentWeapon->SetActorHiddenInGame(false);
	}

	OnWeaponChanged.Broadcast(CurrentWeapon);
}

void AMyRobo::SwitchNextWeapon(EWeaponType TypeToSwitch)
{
	FWeaponTypeInventory* Inventory = GetInventoryForType(TypeToSwitch);

	if (Inventory && Inventory->Weapons.Num() > 0)
	{
		Inventory->CurrentIndex = (Inventory->CurrentIndex + 1) % Inventory->Weapons.Num();

		AWeapon* WeaponToEquip = Inventory->Weapons[Inventory->CurrentIndex];
		EquipWeapon(WeaponToEquip);
	}
}


FWeaponTypeInventory* AMyRobo::GetInventoryForType(EWeaponType WeaponType)
{
	return WeaponInventory.FindByPredicate([WeaponType](const FWeaponTypeInventory& Inventory)
		{
			return Inventory.WeaponType == WeaponType;
		});
}

// Called when the game starts or when spawned
void AMyRobo::BeginPlay()
{
	Super::BeginPlay();

	InteractionWidget->SetHiddenInGame(true);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	if (DefaultMeleeWeaponClass)
	{
		AWeapon* MeleeWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultMeleeWeaponClass, SpawnParams);
		AddWeaponToInventory(MeleeWeapon);
	}

	if (DefaultRangedWeaponClass)
	{
		AWeapon* RangedWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultRangedWeaponClass, SpawnParams);
		AddWeaponToInventory(RangedWeapon);
	}

	if (FWeaponTypeInventory* MeleeInventory = GetInventoryForType(EWeaponType::Melee))
	{
		if (MeleeInventory->Weapons.Num() > 0)
		{
			EquipWeapon(MeleeInventory->Weapons[0]);
		}
	}
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

void AMyRobo::AttackSeaCreature()
{
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

void AMyRobo::SetAcquirableWeapon(AWeapon* Weapon)
{
	AcquirableWeapon = Weapon;
}

void AMyRobo::HandleShortPress()
{
	IsHolding = false;

	//획득 가능한 무기 확인하고 인벤토리에 추가
	if (AcquirableWeapon)
	{
		AddWeaponToInventory(AcquirableWeapon);
		AcquirableWeapon->SetOwner(this); //소유권 설정
		AcquirableWeapon = nullptr; //획득 후 포인터 초기화
	}
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
	if (CurrentWeapon && CurrentWeapon->WeaponStats && CurrentWeapon->WeaponStats->Type == EWeaponType::Ranged)
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
