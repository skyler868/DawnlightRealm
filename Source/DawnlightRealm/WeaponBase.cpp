// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacterBased.h"
#include "Component/HealthComponent.h"
#include "Enemy/EnemyBased.h"
#include "WorldPartition/WorldPartitionSubsystem.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(Mesh);

	

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	HitBox->OnComponentBeginOverlap.AddDynamic(
		this, &AWeaponBase::OnHitBoxOverlap
	);
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OwnerCharacter) return;
	if (OtherActor == OwnerCharacter) return;

	if (HitActors.Contains(OtherActor)) return;
	HitActors.Add(OtherActor);


	UHealthComponent* Health = OtherActor->FindComponentByClass<UHealthComponent>();
	if (!Health) return;

	// 最终伤害 = Player 统一计算
	float Damage;
	if (auto* Player = Cast<APlayerCharacterBased>(OwnerCharacter))
	{
		Damage = Player->GetFinalAttack();
		if (auto* Enemy = Cast<AEnemyBased>(OtherActor)) {
			Enemy->OnHit(Damage);
			Health->ApplyDamage(Damage, OwnerCharacter);
		}
	}
	else if (auto* Enemy = Cast<AEnemyBased>(OwnerCharacter))
	{
		Damage = Enemy->AttackDamage;
		if (auto* PlayerHit = Cast<APlayerCharacterBased>(OtherActor)) {
			PlayerHit->HealthComp->ApplyDamage(Damage, OwnerCharacter);
			
		}
	}
	else {
		return;
	}
	
}

void AWeaponBase::Interact_Implementation(AActor* Interactor)
{
	if (!bCanInteract) return; // 挂在角色身上的武器，直接无效
	
	APlayerCharacterBased* Player = Cast<APlayerCharacterBased>(Interactor);
	if (!Player) return;
	Player->PickupWeapon(this);
}

void AWeaponBase::EnableHitBox()
{
	if (!HitBox) return;
	HitActors.Empty();
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponBase::DisableHitBox()
{
	if (!HitBox) return;

	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::InitFromItem(UWeaponItem* Item)
{
	if (!Item) return;

}

void AWeaponBase::OnEquip(ACharacter* Character)
{
	OwnerCharacter = Character;


	this->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("hand_rSocket")
	);
	if (Mesh) { 
		Mesh->SetSimulatePhysics(false); 
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	bCanInteract = false;
}

void AWeaponBase::OnUnEquip()
{

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Mesh) {

		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	OwnerCharacter = nullptr;
	bCanInteract = true;
	
}

void AWeaponBase::DropToWorld()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetOwner(nullptr); //  防止跟随敌人销毁
	if (Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetSimulatePhysics(true);
	}
	
	//HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OwnerCharacter = nullptr;
	bCanInteract = true;
}
