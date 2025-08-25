// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/SeaCreature.h"
#include "ActorComponent/StateComponent/StateComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/SimpleDamageUI.h"

// Sets default values
ASeaCreature::ASeaCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
	SimpleDamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SimpleDamageWidget"));
	SimpleDamageWidget->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> SimpleDamageWidgetClassFinder(TEXT("/Game/BluePrint/SeaCreature/UI/BP_SimpleDamageUI.BP_SimpleDamageUI_C"));
	if (SimpleDamageWidgetClassFinder.Succeeded())
		SimpleDamageWidget->SetWidgetClass(SimpleDamageWidgetClassFinder.Class);
	SimpleDamageWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts or when spawned
void ASeaCreature::BeginPlay()
{
	Super::BeginPlay();
	StateComponent->OnTakeDamage.BindLambda([this](float Percent)
		{
			USimpleDamageUI* SimpleDamageUI = Cast<USimpleDamageUI>(SimpleDamageWidget->GetWidget());
			if (SimpleDamageUI)
			{
				SimpleDamageUI->SetDamageText(StateComponent->GetDamage());
			}
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
	StateComponent->TakeDamage(DamageAmount);
	/*UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location,
		HitResult.Normal.Rotation(), true);
	if (StateComponent->isDead())
	{
		PlayAnimMontage(DeathMontage);
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]()
			{
				Destroy();
			}, 3.0f, false);
	}
	else
		PlayAnimMontage(HitbyMontage);*/
}

