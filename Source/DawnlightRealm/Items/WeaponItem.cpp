// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponItem.h"
#include "../WeaponBase.h"
#include "../PlayerCharacterBased.h"

UWeaponItem::UWeaponItem()
{
	bConsumable = false;
}

void UWeaponItem::Use(APlayerCharacterBased* Character)
{
	if (!Character) return;
	if (!WeaponActorClass) return;

	// 生成武器
	AWeaponBase* WeaponActor = Character->GetWorld()->SpawnActor<AWeaponBase>(WeaponActorClass);
	if (!WeaponActor) return;

	WeaponActor->InitFromItem(this); // 数据传给WeaponActor
	Character->PickupWeapon(WeaponActor);
}
