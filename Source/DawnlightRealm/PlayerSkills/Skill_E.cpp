// Fill out your copyright notice in the Description page of Project Settings.
#include "Skill_E.h"
#include "../PlayerCharacterBased.h" // 确保路径正确
#include "Animation/AnimInstance.h"
#include "SkillProjectileBase.h"


USkill_E::USkill_E()
{
	Cooldown = 4.f; // 冷却时间
	MPCost = 16.f; // MP耗蓝
	bUnlocked = true; // 是否解锁
}

void USkill_E::Activate(APlayerCharacterBased* Owner)
{
	if (!Owner || !ProjectileClass) return;

	

	if (EMontage) {
		Owner->PlayAnimMontage(EMontage, 1.8f);

		
	}
	FVector SpawnLoc = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 200.f;
	//FVector SpawnLoc = Owner->GetMesh()->GetSocketLocation(TEXT("Hand_R"));
	FVector Dir = Owner->GetActorForwardVector();
	FRotator SpawnRot = Dir.Rotation();
	//FRotator SpawnRot = Owner->GetControlRotation();

	FActorSpawnParameters Params;
	Params.Owner = Owner;
	Params.Instigator = Owner;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	

	ASkillProjectileBase* Projectile = Owner->GetWorld()->SpawnActor<ASkillProjectileBase>(
		ProjectileClass, SpawnLoc, SpawnRot, Params
	);
	
	if (!Projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Skill Projectile"));
	}

	
}
