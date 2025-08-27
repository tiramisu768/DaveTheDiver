// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectUI/DamagePopup.h"
#include "Components/WidgetComponent.h"
#include "UI/SimpleDamageUI.h"

// Sets default values
ADamagePopup::ADamagePopup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SimpleDamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SimpleDamageWidget"));
	SimpleDamageWidget->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FClassFinder<UUserWidget> SimpleDamageWidgetClassFinder(TEXT("/Game/BluePrint/SeaCreature/UI/BP_SimpleDamageUI.BP_SimpleDamageUI_C"));
	if (SimpleDamageWidgetClassFinder.Succeeded())
		SimpleDamageWidget->SetWidgetClass(SimpleDamageWidgetClassFinder.Class);
	SimpleDamageWidget->SetWidgetSpace(EWidgetSpace::Screen);

}

// Called when the game starts or when spawned
void ADamagePopup::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ADamagePopup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamagePopup::HideDamagePopup()
{
	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this, &ADamagePopup::HidePopup, HideInterval, true);
}

void ADamagePopup::HidePopup()
{
	SetActorHiddenInGame(true);
}

void ADamagePopup::GetDamage()
{
	USimpleDamageUI* SimpleDamageUI = Cast<USimpleDamageUI>(SimpleDamageWidget->GetWidget());
	if (SimpleDamageUI)
	{
		//SimpleDamageUI->SetDamageText(FishStateComponent->GetDamage());
	}
}




