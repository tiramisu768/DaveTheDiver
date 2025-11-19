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
		UpdateOpenAnimation(DeltaTime);
	}
	else if(IsRoboOverlap && IsOpen)
	{
		//이미 열린 상자 상태에서의 표시
		GEngine->AddOnScreenDebugMessage(-8, 3.0f, FColor::Purple, TEXT("No Weapon"));
	}
}

void ARandomBox::Interact(AMyRobo* InteractingRobo)
{
	if (IsOpen || IsOpening||!InteractingRobo) return;

	if (IsOpen && IsValid(SpawnedWeapon))
	{
		InteractingRobo->SetAcquirableWeapon(SpawnedWeapon);
		return;
	}

	if (!IsOpen && IsOpening)
	{
		CurrentInteractingRobo = InteractingRobo;
		BeginOpenAnimation();
	}

}

//void ARandomBox::Interact()
//{
//	if (IsOpen) return;
//
//	BeginOpen();
//}

void ARandomBox::ShowInteractionWidget(bool bShow)
{
	// 필요 시 UI 위젯 표시/숨김 로직 구현
}

void ARandomBox::StartFocus()
{
}

void ARandomBox::EndFocus()
{
}



void ARandomBox::RandomBoxOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AMyRobo * Robo = Cast<AMyRobo>(OtherActor))
	{
		IsRoboOverlap = true;
		Robo->SetCurrentInteractable(this);
		Robo->ShowInteractionWidget(true);
		Robo->FocusOnInteractionTarget(this); //카메라 고정
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
			Robo->SetAcquirableWeapon(nullptr);
			Robo->SetCurrentInteractable(nullptr);
			Robo->ShowInteractionWidget(false);
			Robo->FocusOnInteractionTarget(this); //카메라 고정 해제
			CurrentInteractingRobo = nullptr;
		}
	}
}

void ARandomBox::BeginOpenAnimation()
{
	if (IsOpen || IsOpening)
		return;

	IsOpening = true;
	GEngine->AddOnScreenDebugMessage(-6, 2.0f, FColor::Yellow, TEXT("Opening Started"));
}

void ARandomBox::UpdateOpenAnimation(float DeltaTime)
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

		SpawnWeapon();
	}
}

void ARandomBox::SpawnWeapon()
{
	if (!GetWorld()||WeaponClasses.Num()==0) return;

	if (IsValid(SpawnedWeapon)) return;

	const int32 RandomIndex = FMath::RandRange(0, WeaponClasses.Num() - 1);
	TSubclassOf<AWeapon> WeaponToSpawn = WeaponClasses[RandomIndex];

	if (WeaponToSpawn)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50.f);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponToSpawn, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		if (IsValid(SpawnedWeapon)&&IsValid(CurrentInteractingRobo))
		{
			CurrentInteractingRobo->SetAcquirableWeapon(SpawnedWeapon);
		}
	}
}

