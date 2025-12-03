// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SeaCreature/SeaCreature.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bullet Mesh"));
	RootComponent = BulletMesh;
	BulletMesh->SetCollisionProfileName(TEXT("Projectile"));
	BulletMesh->SetNotifyRigidBodyCollision(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->SetUpdatedComponent(BulletMesh);
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 5.0f;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normallmpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		//피격 이펙트(파티클,소리)재생코드추가예정
		ASeaCreature* SeaCreature = Cast<ASeaCreature>(OtherActor);
		if (SeaCreature)
		{
			SeaCreature->HitBy(Damage, Hit);
		}
		//총알파괴
		Destroy();
	}
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

