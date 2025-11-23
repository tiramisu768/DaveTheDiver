// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/RandomBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyRobo/MyRobo.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponData.h"
#include "Engine/DataTable.h"

// Sets default values
ARandomBox::ARandomBox()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BoxCollision->SetBoxExtent(FVector(110.0f, 110.0f, 90.0f));

	BoxFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxFrameMesh"));
	BoxFrameMesh->SetupAttachment(RootComponent);
}

void ARandomBox::ClearSpawnedWeapon()
{
	SpawnedWeapon = nullptr;
	if (CurrentInteractingRobo)
	{
		CurrentInteractingRobo->ShowPickupWidget(false, nullptr);
	}
}

// Called when the game starts or when spawned
void ARandomBox::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARandomBox::RandomBoxOnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARandomBox::RandomBoxOnEndOverlap);

	if(BoxFrameMesh)
	{
		DynMat = BoxFrameMesh->CreateAndSetMaterialInstanceDynamic(0);
	}
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
	if (IsOpen || IsOpening ||!InteractingRobo) return;

	IsOpening = true;
	CurrentInteractingRobo = InteractingRobo;
	GEngine->AddOnScreenDebugMessage(-6, 2.0f, FColor::Yellow, TEXT("Opening Started"));
}

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

		if (IsOpen)
		{
			if (IsValid(SpawnedWeapon))
			{
				Robo->SetAcquirableWeapon(SpawnedWeapon);
				Robo->ShowPickupWidget(true, SpawnedWeapon);
			}
		}
		else
		{
			Robo->SetCurrentInteractable(this);
			Robo->ShowLongPressWidget(true,this);
		}
	}
}

void ARandomBox::RandomBoxOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(AMyRobo * Robo = Cast<AMyRobo>(OtherActor))
	{
		IsRoboOverlap = false;
		Robo->SetAcquirableWeapon(nullptr);
		Robo->SetCurrentInteractable(nullptr);
		Robo->ShowLongPressWidget(false,nullptr);
		Robo->ShowPickupWidget(false,nullptr);
		CurrentInteractingRobo = nullptr;
	}
}

void ARandomBox::UpdateOpenAnimation(float DeltaTime)
{
	if (!DynMat) return;

	Brightness = FMath::Max(0.03f, Brightness - DeltaTime * 2.0f);
	DynMat->SetScalarParameterValue(TEXT("Brightness"), Brightness);

	// 밝기가 최소치에 도달하면 Open 상태로 전환
	if (Brightness <= 0.03f)
	{
		IsOpen = true;
		IsOpening = false;

		SpawnWeapon();
	}
}

void ARandomBox::SpawnWeapon()
{
	if (!WeaponDataTable || WeaponDataTable->GetRowMap().Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No WeaponData"));
		return;
	}
// 이미 무기가 스폰되었다면 함수를 종료합니다.
	if (IsValid(SpawnedWeapon)) return;
// 2. 데이터 테이블의 모든 행 이름(Row Name)을 가져옵니다.
	TArray<FName> RowNames = WeaponDataTable->GetRowNames();
// 3. 행 이름 중 하나를 무작위로 선택합니다.
	const FName RandomRowName = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
// 4. 선택된 행 이름으로 데이터 테이블에서 실제 데이터(FWeaponData)를 찾아옵니다.
	static const FString ContextString(TEXT("WeaponDataTable Context"));
	FWeaponData* FoundWeaponData = WeaponDataTable->FindRow<FWeaponData>(RandomRowName, ContextString);
// 5. 데이터와 데이터 안의 WeaponClass가 유효한지 확인하고 무기를 스폰합니다.
	if (FoundWeaponData && FoundWeaponData->WeaponClass)
	{
		TSubclassOf<AWeapon> WeaponToSpawn = FoundWeaponData->WeaponClass;

		FVector SpawnLocation = GetActorLocation();

		FVector DirectionToRobo = CurrentInteractingRobo->GetActorLocation() - GetActorLocation();
		DirectionToRobo.Z = 0;
		DirectionToRobo.Normalize();
		SpawnLocation += DirectionToRobo * 60.0f + FVector(0, 0, 100.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponToSpawn, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		if (IsValid(SpawnedWeapon))
		{
			SpawnedWeapon->RowName = RandomRowName;
			SpawnedWeapon->PostInitializeComponents();

			UE_LOG(LogTemp, Log, TEXT("[RandomBox] Weapon Spawning: %s with RowName: %s"), * SpawnedWeapon->GetName(), * RandomRowName.ToString());

			if(IsValid(CurrentInteractingRobo))
			{
				CurrentInteractingRobo->SetAcquirableWeapon(SpawnedWeapon);
				CurrentInteractingRobo->ShowPickupWidget(true, SpawnedWeapon);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[RandomBox] Spawn failed! Row '%s' in DataTable has a null WeaponClass!"), *RandomRowName.ToString());
	}
}

