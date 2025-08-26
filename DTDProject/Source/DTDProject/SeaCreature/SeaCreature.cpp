// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/SeaCreature.h"
#include "ActorComponent/StateComponent/FishStateComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/SimpleDamageUI.h"
#include "Object/ObjectUI/DamagePopup.h"

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

}

// Called to bind functionality to input
void ASeaCreature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASeaCreature::HitBy(float DamageAmount)
{
	/*if (StateComponent->isDead() || HitbyMontage == nullptr)
		return;*/
	if (FishStateComponent->isDead()) return;
	FishStateComponent->TakeDamage(DamageAmount);
	SpawnDamagePopup();
	/*UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location,
		HitResult.Normal.Rotation(), true);*/
	if (FishStateComponent->isDead())
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.0f, FColor::Red, FString::Printf(TEXT("isDead")));
		/*PlayAnimMontage(DeathMontage);
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]()
			{
				Destroy();
			}, 3.0f, false);*/
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.0f, FColor::Red, FString::Printf(TEXT("Not isDead")));
		//PlayAnimMontage(HitbyMontage);
	}


}

void ASeaCreature::SpawnDamagePopup()
{
	GEngine->AddOnScreenDebugMessage(-6, 5.0f, FColor::Red, FString::Printf(TEXT("SpawnDamagePopup")));
	ADamagePopup* damagePopup = GetWorld()->SpawnActor<ADamagePopup>(SpawnDamagePopupClass, GetActorLocation(), GetActorRotation(), FActorSpawnParameters());
	if (nullptr == damagePopup) return;
	//damagePopup->InitStat(*Data);

	//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::SpawnMonster, SpawnInterval, true);
}

