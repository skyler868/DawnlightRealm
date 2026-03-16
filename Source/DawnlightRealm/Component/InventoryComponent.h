// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAWNLIGHTREALM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UItemBase*> Items; // 背包物品列表

	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemBase* Item); // 添加物品

	UFUNCTION(BlueprintCallable)
	void UseItem(int32 SlotIndex); // 使用物品

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 SlotIndex); // 移除物品


	UFUNCTION(BlueprintCallable)
	void SwapItems(int32 A, int32 B);


	UPROPERTY(BlueprintReadOnly)
	int32 MaxSlots;
		
};
