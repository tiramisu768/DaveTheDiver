// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/RandomBox.h"
#include "RandomBox.h"
#include "Components/BoxComponent.h"
#include "MyRobo/MyRobo.h"

// Sets default values
ARandomBox::ARandomBox()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxFrameMesh"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	//BoxCollision->SetupAttachment(BoxFrameMesh);


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

	if (IsOpening)
	{
		OpenRandomBox(DeltaTime);
	}
	else if(IsRoboOverlap && IsOpen)
	{
		//이미 열린 상자 상태에서의 표시
		GEngine->AddOnScreenDebugMessage(-8, 3.0f, FColor::Purple, TEXT("No Weapon"));
	}
}

void ARandomBox::Interact()
{
	if (IsOpen) return;

	BeginOpen();
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
			Robo->ShowInteractionWidget(true);
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
			Robo->ShowInteractionWidget(false);
			Robo->FocusOnInteractionTarget(this); //카메라 고정 해제
		}
	}
}

void ARandomBox::BeginOpen()
{
	if (IsOpen || IsOpening)
		return;

	IsOpening = true;
	GEngine->AddOnScreenDebugMessage(-6, 2.0f, FColor::Yellow, TEXT("Opening Started"));
}

void ARandomBox::OpenRandomBox(float DeltaTime)
{
	if (!DynMat) return;

	Brightness = FMath::Max(0.03f, Brightness - DeltaTime * 2.0f);
	DynMat->SetScalarParameterValue(TEXT("Brightness"),Brightness);

	// 밝기가 최소치에 도달하면 Open 상태로 전환
	if (Brightness <= 0.03f)
	{
		IsOpen = true;
		IsOpening = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Box Opened!"));

		SpawnWeaponOnBox();
	}
}

void ARandomBox::SpawnWeaponOnBox()
{
	if (!GetWorld()) return;

	FVector SpawnLocation = BoxFrameMesh->GetComponentLocation() + FVector(0, 0, 50);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	TSubclassOf<AWeapon> WeaponClass = TestWeaponClass;
	if (WeaponClass)
	{
		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnLocation, SpawnRotation);
		//if (SpawnedWeapon)
		//{
		//	SpawnedWeapon->AttachToComponent(BoxFrameMesh, FAttachmentTransformRules::KeepWorldTransform);
		//}
	}
}

