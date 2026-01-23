// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/BaseBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyRobo/MyRobo.h"
#include "LootSpawnerBox.h"

ABaseBox::ABaseBox()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	BoxCollision->SetBoxExtent(FVector(110.0f, 110.0f, 90.0f));

	BoxFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxFrameMesh"));
	BoxFrameMesh->SetupAttachment(RootComponent);
}

void ABaseBox::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseBox::OnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseBox::OnEndOverlap);

	if (BoxFrameMesh)
	{
		for (int32 i = 0; i < BoxFrameMesh->GetNumMaterials(); ++i)
		{
			UMaterialInstanceDynamic* DynMat = BoxFrameMesh->CreateAndSetMaterialInstanceDynamic(i);

			if (DynMat)
			{
				DynMat->SetScalarParameterValue(TEXT("Brightness"), Brightness);
				DynMats.Add(DynMat);
			}
		}
	}

}

void ABaseBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpening)
	{
		UpdateOpenAnimation(DeltaTime);
	}
}

void ABaseBox::Interact(AMyRobo* InteractingRobo)
{
	if (IsOpen || IsOpening || !InteractingRobo) return;

	IsOpening = true;
	CurrentInteractingRobo = InteractingRobo;
}

void ABaseBox::ShowInteractionWidget(bool bShow)
{
}

void ABaseBox::StartFocus()
{
}

void ABaseBox::EndFocus()
{
}

void ABaseBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyRobo* Robo = Cast<AMyRobo>(OtherActor))
	{
		IsRoboOverlap = true;
		if (!IsOpen)
		{
			Robo->SetCurrentInteractable(this);
			Robo->ShowLongPressWidget(true, this);
		}
	}
}

void ABaseBox::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMyRobo* Robo = Cast<AMyRobo>(OtherActor))
	{
		IsRoboOverlap = false;
		Robo->SetCurrentInteractable(nullptr);
		Robo->ShowLongPressWidget(false, nullptr);
		CurrentInteractingRobo = nullptr;
	}
}

void ABaseBox::UpdateOpenAnimation(float DeltaTime)
{
    if (DynMats.Num()==0) return;

	Brightness = FMath::Max(0.03f, Brightness - DeltaTime * OpenSpeed);

	for (UMaterialInstanceDynamic* DynMat : DynMats)
	{
		if (DynMat)
		{
			DynMat->SetScalarParameterValue(TEXT("Brightness"), Brightness);
		}
	}
	// 밝기가 최소치에 도달하면 Open 상태로 전환
	if (Brightness <= 0.03f)
	{
		IsOpen = true;
		IsOpening = false;
		OnBoxOpened();
	}
}

