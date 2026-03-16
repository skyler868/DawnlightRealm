// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "../Component/HealthComponent.h"
#include "../Enemy/EnemyBased.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ASkillProjectileBase::ASkillProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision; //Actor 的位置 = RootComponent 的位置
	Collision->SetCollisionProfileName("Projectile");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ProjectileMovement->SetUpdatedComponent(Collision); //UProjectileMovementComponent推Collision。
	//Collision->OnComponentBeginOverlap.AddDynamic(this, &ASkillProjectileBase::OnOverlap); //注册“碰撞回调”

	Collision->OnComponentHit.AddDynamic(this, &ASkillProjectileBase::OnHit);
}

// Called when the game starts or when spawned
void ASkillProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.0f); // 5 秒后自动 Destroy
	Collision->IgnoreActorWhenMoving(GetOwner(), true);
}

// Called every frame
void ASkillProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ASkillProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (!IsValid(OtherActor)) {
//	
//		return;
//	}
//
//	if (OtherActor == this || OtherActor == GetOwner()) return;
//
//	if (AEnemyBased* Enemy = Cast<AEnemyBased>(OtherActor)) {
//		Enemy->OnHit(Damage);
//		if (UHealthComponent* Health = Enemy->FindComponentByClass<UHealthComponent>())
//		{
//			Health->ApplyDamage(Damage, GetOwner());
//		}
//
//	}
//	if (HitEffect) {
//		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//			GetWorld(), HitEffect, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());
//	}
//	Destroy();
//}

void ASkillProjectileBase::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!IsValid(OtherActor) || OtherActor == GetOwner())
		return;

	if (AEnemyBased* Enemy = Cast<AEnemyBased>(OtherActor))
	{
		if (UHealthComponent* Health = Enemy->FindComponentByClass<UHealthComponent>())
		{
			Health->ApplyDamage(Damage, GetOwner());
		}
	}

	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation());
	}

	Destroy();
}


