// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/SeaCreature.h"
#include "ActorComponent/StateComponent/FishStateComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/SimpleDamageUI.h"
#include "Object/ObjectUI/DamagePopup.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASeaCreature::ASeaCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FishStateComponent = CreateDefaultSubobject<UFishStateComponent>(TEXT("FishStateComponent"));
	FishHPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FishHPBarWidget"));
	FishHPBarWidget->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> FishHPBarWidgetClassFinder(TEXT(""));
	if (FishHPBarWidgetClassFinder.Succeeded())
		FishHPBarWidget->SetWidgetClass(FishHPBarWidgetClassFinder.Class);
	FishHPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);


	static ConstructorHelpers::FObjectFinder<UAnimMontage>HitbyMontageObjectFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/SeaCreature/Animation/AM_Hitby.AM_Hitby'"));
	if (HitbyMontageObjectFinder.Succeeded())
		HitbyMontage = HitbyMontageObjectFinder.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DeathMontageObjectFinder(TEXT("/Script/Engine.AnimMontage'/Game/BluePrint/SeaCreature/Animation/AM_Death.AM_Death'"));
	if (DeathMontageObjectFinder.Succeeded())
		DeathMontage = DeathMontageObjectFinder.Object;
	//AIControllerClass = AMonsterAIController::StaticClass();
	////EAutoPossessAI
	////Disabled, //AIController사용안함
	////PlacedInWorld.//게임 시작시 배치되어있는 pawn은 AIController를 소유함
	////Spawned, //Spawn된 Pawn은 AIController를 소유함
	////PlacedInWorldOrSpawned, //게임 시작시 배치되어있는 Pawn과 Spawn된 Pawn은 AIController를 소유함
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageObjectFinder(TEXT(""));
	if (AttackMontageObjectFinder.Succeeded())
		AttackMontage = AttackMontageObjectFinder.Object;
}

// Called when the game starts or when spawned
void ASeaCreature::BeginPlay()
{
	Super::BeginPlay();
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
}

// Called every frame
void ASeaCreature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FishStateComponent->isDead())
		AddMovementInput(Forward);
}

// Called to bind functionality to input
void ASeaCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASeaCreature::HitBy(float DamageAmount, const FHitResult& HitResult)
{
	if (FishStateComponent->isDead() || HitbyMontage == nullptr)
		return;

	FishStateComponent->TakeDamage(DamageAmount);
	SpawnDamagePopup(DamageAmount);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location,
		HitResult.Normal.Rotation(), true);
	if (FishStateComponent->isDead())
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.0f, FColor::Red, FString::Printf(TEXT("isDead")));
		PlayAnimMontage(DeathMontage);
		////////20초 후 자동삭제로 하고 20초 내에 로보가 물고기에 부딪히면 수확 및 삭제////////
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]()
			{
				Destroy();
			}, 3.0f, false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.0f, FColor::Red, FString::Printf(TEXT("Not isDead")));
		PlayAnimMontage(HitbyMontage);
	}


}

bool ASeaCreature::isDead()
{
	return FishStateComponent->isDead();
}

void ASeaCreature::Attack(AMyRobo* Target)
{
	////GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Monster Attack!"));
	//if (FishStateComponent->isDead() || AttackMontage == nullptr || Target == nullptr)
	//	return;
	//if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
	//	return;
	//FVector TargetDirection = Target->GetActorLocation() - GetActorLocation();
	//FRotator LookAtRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
	//SetActorRotation(LookAtRotation);
	//PlayAnimMontage(AttackMontage);
}

void ASeaCreature::SpawnDamagePopup(float DamageAmount)
{
	/*float HeightOffset = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 0.3f;
	FVector SpawnLoc = GetActorLocation() + FVector(0.f, 0.f, HeightOffset);*/
	ADamagePopup* damagePopup = GetWorld()->SpawnActor<ADamagePopup>(SpawnDamagePopupClass, GetActorLocation(), GetActorRotation(), FActorSpawnParameters());
	if (nullptr == damagePopup) return;

	USimpleDamageUI* SimpleDamageUI = Cast<USimpleDamageUI>(damagePopup-> SimpleDamageWidget->GetWidget());
	if (SimpleDamageUI)
	{
		SimpleDamageUI->SetDamageText(FishStateComponent->GetDamage());
	}
	//////////////추후 오브젝트풀링으로 수정/////////////////
	damagePopup->HideDamagePopup();

	
}

