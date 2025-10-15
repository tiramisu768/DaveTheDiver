// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/RandomBox.h"
#include "RandomBox.h"
#include "Components/BoxComponent.h"

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

}

// Called every frame
void ARandomBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARandomBox::Interact()
{
	IsOpen = true;
	GEngine->AddOnScreenDebugMessage(-6, 3.0f, FColor::Purple, TEXT("IsOpen"));
}

void ARandomBox::RandomBoxOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, FString::Printf(TEXT("OnBeginOverlap")) + OtherActor->GetName());

	//if (OtherActor->IsA(AYourPlayerCharacter::StaticClass()))
	//{
	//	// 카메라 고정
	//	// UI 표시
	//	// 상호작용 가능 상태로 변경
	//}
}

void ARandomBox::RandomBoxOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-7, 3.0f, FColor::Purple, FString::Printf(TEXT("OnEndOverlap")) + OtherActor->GetName());
}

