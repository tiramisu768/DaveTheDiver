// Fill out your copyright notice in the Description page of Project Settings.


#include "SeaCreature/SeaCreature.h"
#include "ActorComponent/StateComponent/StateComponent.h"

// Sets default values
ASeaCreature::ASeaCreature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));
}

// Called when the game starts or when spawned
void ASeaCreature::BeginPlay()
{
	Super::BeginPlay();
	StateComponent->OnTakeDamage.BindLambda([this](float Percent)
		{
			GEngine->AddOnScreenDebugMessage(-4, 5.f, FColor::Orange, FString::Printf(TEXT("HP Percent: %f"), Percent));
			//UHPBarUI* HPBarUI = Cast<UHPBarUI>(HPBarWidget->GetWidget());
			//if (HPBarUI)
			//{
			//	HPBarUI->SetHPBarPercent(Percent);
			//}
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
		return;
	StateComponent->TakeDamage(DamageAmount);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location,
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

