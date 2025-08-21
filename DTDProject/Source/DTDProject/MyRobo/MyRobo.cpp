// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRobo/MyRobo.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "UI/RoboHPBarUI.h"		
#include "HUD/MyHUD.h"
#include "BuoyancyComponent.h"
#include "WaterBodyComponent.h"

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
	WeaponComponent->SetupAttachment(BodyComponent, FName(TEXT("Weapon")));

	//SpringArm->bUsePawnControlRotation = true;
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);  // 회전 속도
#pragma endregion

#pragma region	connect AnimMontage

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAttackMontageFinder(TEXT("/Script/Engine.AnimSequence'/Game/BluePrint/MyRobo/Animation/Stabbing__1_.Stabbing__1_'"));
	if (MeleeAttackMontageFinder.Succeeded())
	{
		MeleeAttackMontage = MeleeAttackMontageFinder.Object;
	}
#pragma endregion

	RoboComponent = CreateDefaultSubobject<URoboComponent>(TEXT("RoboComponent"));
	//InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	//InteractionWidget->SetupAttachment(GetRootComponent());
	//static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetClassFinder(TEXT("/Game/Blueprints/UI/BP_InteractionUI.BP_InteractionUI_C"));
	//if (InteractionWidgetClassFinder.Succeeded())
	//	InteractionWidgetClass = InteractionWidgetClassFinder.Class;
	//InteractionWidget->SetWidgetClass(InteractionWidgetClass);
}

// Called when the game starts or when spawned
void AMyRobo::BeginPlay()
{
	Super::BeginPlay();

	RoboComponent->InitRoboUIStatement();

	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Swimming);
}

// Called every frame
void AMyRobo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (InteractionObject)
	//{
	//	InteractionWidget->SetHiddenInGame(false);
	//	AActor* InteractionActor = Cast<AActor>(InteractionObject);
	//	if (InteractionActor)
	//		InteractionWidget->SetWorldLocation(InteractionActor->GetActorLocation());
	//}
	//else
	//	InteractionWidget->SetHiddenInGame(true);

	//DepthBelowSurface = GetDepthBelowSurface();

	if (BuoyancyComponent->GetCurrentWaterBodyComponents().IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Blue, TEXT("Out Sea"));
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Blue, TEXT("In Sea"));
	}


	bool bInWater = !BuoyancyComponent->GetCurrentWaterBodyComponents().IsEmpty();
	//const bool bInWater = DepthBelowSurface > 0.f;

	if (bInWater)
	{
		if (GetCharacterMovement()->MovementMode != MOVE_Swimming)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Swimming"));
		}
	}
	else
	{
		if (GetCharacterMovement()->MovementMode != MOVE_Walking)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Walking"));
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

	APlayerController* controller = Cast<APlayerController>(NewController);
	AMyHUD* MyHUD = Cast<AMyHUD>(controller->GetHUD());
	//델리게이트 등록
	RoboComponent->OnHPChanged.BindLambda([this, MyHUD](float value) {
		MyHUD->SetHPPercent(value);
		});
	RoboComponent->OnDepthChanged.BindLambda([this, MyHUD](float value) {
		MyHUD->SetMeters(value);
		});
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
		if (GetMovementComponent()->IsFalling() == true || nullptr == MeleeAttackMontage || isMeleeAttack == false
			|| BodyComponent->GetAnimInstance()->Montage_IsPlaying(MeleeAttackMontage))
			return;
		PlayMontageFullBody(MeleeAttackMontage);
		/*PlayMontageFullBody(MeleeAttackMontage, AttackSectionNames[AttackIndex]);
		++AttackIndex;
		AttackIndex %= AttackSectionNames.Num();*/
}

void AMyRobo::WeaponActive()
{
	isEquip = true;
	WeaponComponent->AttachToComponent(BodyComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
	FName(TEXT("Weapon")));
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

#pragma region reference


//void AMyCharacter::InteractionAction()
//{
//	if (InteractionObject)
//		InteractionObject->Interact();
//}
//
//void AMyCharacter::AttackTrace()
//{
//	TArray<FHitResult> HitResult;
//	bool isHit = UKismetSystemLibrary::BoxTraceMulti(
//		this,
//		GetActorLocation(),
//		GetActorLocation() + GetActorForwardVector() * 100.0f,
//		FVector(50.0f, 50.0f, 50.0f),
//		FRotator::ZeroRotator,
//		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel6),
//		false,
//		{},
//		EDrawDebugTrace::ForDuration,
//		HitResult,
//		true
//	);
//	if (isHit)
//	{
//		for (const FHitResult& result : HitResult)
//		{
//			AMonster* Monster = Cast<AMonster>(result.GetActor());
//			if (Monster != nullptr)
//			{
//				Monster->HitBy(StateComponent->GetDamage(), result);
//			}
//		}
//	}
//}
//
//void AMyCharacter::HitBy(float DamageAmount)
//{
//	StateComponent->TakeDamage(DamageAmount);
//}
//


#pragma endregion

