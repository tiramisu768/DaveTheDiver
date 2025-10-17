// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/RandomBox.h"
#include "RandomBox.h"
#include "Components/BoxComponent.h"
#include "MyRobo/MyRobo.h"

// Sets default values
ARandomBox::ARandomBox()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxFrameMesh"));

}

// Called when the game starts or when spawned
void ARandomBox::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARandomBox::RandomBoxOnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARandomBox::RandomBoxOnEndOverlap);

	UStaticMeshComponent* MeshComp = FindComponentByClass<UStaticMeshComponent>();
	DynMat = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMat)
	{
		// 초기 Brightness 값 설정
		DynMat->SetScalarParameterValue(TEXT("Brightness"), Brightness);
	}

}

// Called every frame
void ARandomBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsRoboOverlap)
	{
		if (!IsOpen)
		{
			OpenRandomBox(DeltaTime);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-8, 3.0f, FColor::Purple, TEXT("No Weapon"));
		}
	}
}

void ARandomBox::Interact()
{
	//이미 열어본 상자이면 return
	if (IsOpen) return;

	IsOpen = true;
	GEngine->AddOnScreenDebugMessage(-6, 3.0f, FColor::Purple, TEXT("IsOpen is True"));
}

void ARandomBox::RandomBoxOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AMyRobo* Robo = Cast<AMyRobo>(OtherActor);
		if (Robo)
		{
			IsRoboOverlap = true;
			Robo->SetCurrentInteractable(this);
			Robo->ShowInteractionWidget(true, 0.0f);
			Robo->FocusOnInteractionTarget(this); //카메라 고정
		}
	}
}

void ARandomBox::RandomBoxOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		AMyRobo* Robo = Cast<AMyRobo>(OtherActor);
		if (Robo)
		{
			IsRoboOverlap = false;
			Robo->SetCurrentInteractable(nullptr);
			Robo->ShowInteractionWidget(false,0.0f);
			Robo->FocusOnInteractionTarget(this); //카메라 고정
		}
	}
}

void ARandomBox::OpenRandomBox(float DeltaTime)
{
	if (!DynMat) return;

	Brightness = FMath::Max(0.03f, Brightness - DeltaTime * 2.0f);
	if (DynMat)
		DynMat->SetScalarParameterValue(TEXT("Brightness"),Brightness);

	// 밝기가 최소치에 도달하면 Open 상태로 전환
	if (Brightness <= 0.03f)
	{
		IsOpen = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Box Opened!"));
	}
}

