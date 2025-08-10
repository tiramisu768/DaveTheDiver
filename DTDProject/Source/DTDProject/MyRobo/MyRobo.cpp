// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRobo/MyRobo.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "HUD/MyHUD.h"

// Sets default values
AMyRobo::AMyRobo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#pragma region Init
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	BodyComponent = GetMesh();
	BodyComponent->SetupAttachment(GetRootComponent());
#pragma endregion

	#pragma region	connect AnimMontage
	//BodyComponent = GetMesh();
	//HeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadComponent"));
	//HeadComponent->SetupAttachment(BodyComponent);
	//ArmComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmComponent"));
	//ArmComponent->SetupAttachment(BodyComponent);
	//LegComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegComponent"));
	//LegComponent->SetupAttachment(BodyComponent);
	//FootComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FootComponent"));
	//FootComponent->SetupAttachment(BodyComponent);
	//HairComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairComponent"));
	//HairComponent->SetupAttachment(HeadComponent);

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

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/MyCharacter/Animation/AM_Jump.AM_Jump'"));
	//if (JumpMontageFinder.Succeeded())
	//{
	//	JumpMontage = JumpMontageFinder.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/MyCharacter/Animation/AM_Equip.AM_Equip'"));
	//if (EquipMontageFinder.Succeeded())
	//{
	//	EquipMontage = EquipMontageFinder.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/MyCharacter/Animation/AM_Attack.AM_Attack'"));
	//if (AttackMontageFinder.Succeeded())
	//{
	//	AttackMontage = AttackMontageFinder.Object;
	//}

	//InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	//InteractionWidget->SetupAttachment(GetRootComponent());
	//static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetClassFinder(TEXT("/Game/Blueprints/UI/BP_InteractionUI.BP_InteractionUI_C"));
	//if (InteractionWidgetClassFinder.Succeeded())
	//	InteractionWidgetClass = InteractionWidgetClassFinder.Class;
	//InteractionWidget->SetWidgetClass(InteractionWidgetClass);

	//StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
	//PlayerHPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerHPBarWidget"));
	////PlayerHPBarWidget->SetupAttachment(GetRootComponent());
	//static ConstructorHelpers::FClassFinder<UUserWidget> PlayerHPBarWidgetClassFinder(TEXT("/Game/Blueprints/UI/BP_HPBar.BP_HPBar_C"));
	//if (PlayerHPBarWidgetClassFinder.Succeeded())
	//	PlayerHPBarWidget->SetWidgetClass(PlayerHPBarWidgetClassFinder.Class);
	//PlayerHPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
#pragma endregion
}

// Called when the game starts or when spawned
void AMyRobo::BeginPlay()
{
	Super::BeginPlay();
	//InteractionWidget->SetHiddenInGame(true);
	//UPlayerHPBarUI* PlayerHPBarUI = Cast<UPlayerHPBarUI>(PlayerHPBarWidget->GetWidget());
	//if (PlayerHPBarUI)
	//{
	//	PlayerHPBarUI->SetHPBarPercent(StateComponent->GetHPPercent());
	//}
	//StateComponent->InitHP();

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

#pragma region reference
//void AMyCharacter::PlayJumpMontage()
//{
//	//nullptr != JumpMontage 왜 이렇게 쓰는거지
//	if (GetMovementComponent()->IsFalling() == false && nullptr != JumpMontage &&
//		BodyComponent->GetAnimInstance()->Montage_IsPlaying(JumpMontage) == false)
//	{
//		PlayMontageFullBody(JumpMontage);
//	}
//}
//
//void AMyCharacter::PlayMontageFullBody(TObjectPtr<UAnimMontage> Montage, FName SectionName)
//{
//	if (Montage == nullptr) return;
//	BodyComponent->GetAnimInstance()->Montage_Play(Montage);
//	HeadComponent->GetAnimInstance()->Montage_Play(Montage);
//	ArmComponent->GetAnimInstance()->Montage_Play(Montage);
//	LegComponent->GetAnimInstance()->Montage_Play(Montage);
//	FootComponent->GetAnimInstance()->Montage_Play(Montage);
//	HairComponent->GetAnimInstance()->Montage_Play(Montage);
//	if (SectionName.IsNone() == false)
//	{
//		BodyComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
//		HeadComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
//		ArmComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
//		LegComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
//		FootComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
//		HairComponent->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
//	}
//}
//
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
//void AMyCharacter::PlayAttackMontage()
//{
//	if (GetMovementComponent()->IsFalling() == true || nullptr == AttackMontage || isEquip == false
//		|| BodyComponent->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
//		return;
//	PlayMontageFullBody(AttackMontage, AttackSectionNames[AttackIndex]);
//	++AttackIndex;
//	AttackIndex %= AttackSectionNames.Num();
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
//controller 생성이후 호출되는 함수
void AMyRobo::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	APlayerController* controller = Cast<APlayerController>(NewController);
	AMyHUD* MyHUD = Cast<AMyHUD>(controller->GetHUD());
	/*StateComponent->OnTakeDamage.BindLambda([this, MyHUD](float value) {
		MyHUD->SetHPPercent(value);
		});*/
}
#pragma endregion

