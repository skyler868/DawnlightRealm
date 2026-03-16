// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "../PlayerCharacterBased.h"
#include "ConsumableItem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UConsumableItem : public UItemBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealAmount = 50.f;


	virtual void Use(APlayerCharacterBased* Character) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Use_BP(APlayerCharacterBased* Character); //Ð´À¶Í¼Âß¼­
};
