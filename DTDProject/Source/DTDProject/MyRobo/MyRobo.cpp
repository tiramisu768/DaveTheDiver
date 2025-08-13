// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRobo/MyRobo.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "UI/RoboHPBarUI.h"		
#include "HUD/MyHUD.h"
#include "BuoyancyComponent.h"

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

	//WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	//WeaponComponent->SetupAttachment(BodyComponent, FName(TEXT("Weapon")));
	//WeaponCoverComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponHand"));
	//WeaponCoverComponent->SetupAttachment(BodyComponent, FName(TEXT("Weapon")));

	//SpringArm->bUsePawnControlRotation = true;
	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);  // 회전 속도
#pragma endregion

#pragma region	connect AnimMontage

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageFinder(TEXT(""));
	if (AttackMontageFinder.Succeeded())
	{
		MeleeAttackMontage = AttackMontageFinder.Object;
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

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Swimming);
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

	if (BuoyancyComponent->GetCurrentWaterBodyComponents().IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Blue, TEXT("Out Sea"));
	}
	else
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Blue, TEXT("In Sea"));
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
	RoboComponent->OnO2Changed.BindLambda([this, MyHUD](float value) {
		MyHUD->SetHPPercent(value);
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
	//	if (GetMovementComponent()->IsFalling() == true || nullptr == AttackMontage || isEquip == false
	//		|| BodyComponent->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
	//		return;
	//	PlayMontageFullBody(AttackMontage, AttackSectionNames[AttackIndex]);
	//	++AttackIndex;
	//	AttackIndex %= AttackSectionNames.Num();
}

#pragma region reference

//void AMyCharacter::PlayEquipWeaponMontage()
//{
//	if (GetMovementComponent()->IsFalling() == true || nullptr == EquipMontage ||
//		BodyComponent->GetAnimInstance()->Montage_IsPlaying(EquipMontage) == true ||
//		BodyComponent->GetAnimInstance()->Montage_IsPlaying(AttackMontage) == true)
//		return;
//
//	if (isEquip == false)
//		PlayMontageFullBody(EquipMontage, "Equip");
//	else
//		PlayMontageFullBody(EquipMontage, "UnEquip");
//}
//
//void AMyCharacter::WeaponEquip()
//{
//	isEquip = true;
//	WeaponComponent->AttachToComponent(BodyComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
//		FName(TEXT("WeaponHand")));
//}
//
//void AMyCharacter::WeaponUnEquip()
//{
//	isEquip = false;
//	WeaponComponent->AttachToComponent(BodyComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
//		FName(TEXT("Weapon")));
//}
//
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

