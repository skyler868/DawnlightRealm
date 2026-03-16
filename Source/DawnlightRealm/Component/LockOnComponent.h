// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

class APlayerCharacterBased;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAWNLIGHTREALM_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY() // 当前锁定目标
	AActor* LockedTarget = nullptr;

	UPROPERTY(EditAnywhere, Category="LockOn")
	float SearchRadius = 1500.f; // 有效距离

	void FindTarget(); // 找目标

	void ClearTarget(); // 清空目标

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) // 按键调用
	void ToggleLockOn();  // 锁敌


};
