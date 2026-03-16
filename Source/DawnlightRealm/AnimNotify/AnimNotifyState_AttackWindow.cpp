// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackWindow.h"
#include "../PlayerCharacterBased.h"
#include "../WeaponBase.h"

void UAnimNotifyState_AttackWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp) return;

	APlayerCharacterBased* Player = Cast<APlayerCharacterBased>(MeshComp->GetOwner());
	if (!Player) return;

	AWeaponBase* Weapon = Player->GetCurrentWeapon();
	if (!Weapon) return;

	Weapon->EnableHitBox();  //攻击动画开始打开武器碰撞
}

void UAnimNotifyState_AttackWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	APlayerCharacterBased* Player = Cast<APlayerCharacterBased>(MeshComp->GetOwner());
	if (!Player) return;

	AWeaponBase* Weapon = Player->GetCurrentWeapon();
	if (!Weapon) return;

	Weapon->DisableHitBox(); //攻击动画结束关闭武器碰撞
}
