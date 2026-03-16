// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponItem.generated.h"

class AWeaponBase;

UCLASS(BlueprintType)
class DAWNLIGHTREALM_API UWeaponItem : public UItemBase
{
	GENERATED_BODY()
	
public:
	UWeaponItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponActorClass;

	virtual void Use(class APlayerCharacterBased* Character) override;
};
