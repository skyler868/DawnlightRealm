// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;  // ID

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName; // 物体名字

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon; // 物体图片

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description; // 物体描述

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bConsumable = false; // 是否是消耗品

	UFUNCTION(BlueprintCallable)
	virtual void Use(class APlayerCharacterBased* Character);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	bool bStackable = false; // 是否可以堆叠

	UPROPERTY(BlueprintReadOnly)
	int32 Count = 1;

	int32 MaxStack = 999;

};
