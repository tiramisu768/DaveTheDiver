// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/PickupItem.h"
#include "Components/SphereComponent.h"
#include "MyRobo/MyRobo.h"

// Sets default values
APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShpereComp"));
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	AutoPickuponOverlap = true;
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComp)
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnShpereBeginOverlap);
	}
	
}

void APickupItem::OnShpereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	AMyRobo* Robo = Cast<AMyRobo>(OtherActor);
	if (!Robo) return;

	Robo->ShowPickupWidget(true, this);

	if (AutoPickuponOverlap)
	{
		Robo->ShowPickupWidget(false, nullptr);

		OnPickedUp(Robo);

		FinishPickup();
	}
}

void APickupItem::OnPickedUp(AMyRobo* Robo)
{
}

void APickupItem::OnPickedUP_Implementation(AMyRobo* Robo)
{
	if (!Robo) return;

	switch (ItemType)
	{
	case EPickupItemType::Ammo:
		//Robo->ReceiveAmmoFromPickup(AmmoAmount);
		break;

	case EPickupItemType::Tool:
		/*if (Robo->GetInventoryComponent())
		{
			bool Added = Robo->GetInventoryComponent()->TryAddTool(ToolClass);
			if (!Added)
			{
				Robo->PromptToolReplace(ToolClass);
			}
		}*/
		break;
	}
}

void APickupItem::FinishPickup()
{
	Destroy();
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

