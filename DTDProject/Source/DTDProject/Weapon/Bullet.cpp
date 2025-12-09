// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SeaCreature/SeaCreature.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = CollisionComp;
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->SetNotifyRigidBodyCollision(false);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);

	BulletMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bullet Mesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->SetUpdatedComponent(BulletMesh);
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 500.0f;
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		//피격 이펙트(파티클,소리)재생코드추가예정
		ASeaCreature* SeaCreature = Cast<ASeaCreature>(OtherActor);
		if (SeaCreature)
		{
			SeaCreature->HitBy(Damage, SweepResult);
			Destroy();
		}
		// (선택) 물고기가 아닌 다른 벽 같은 것에 부딪혔을 때도 파괴하고 싶다면 아래 코드를 활성화
		// else 
		// {
		//     Destroy();
		// }
	}
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

