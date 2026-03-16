// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHP); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnMaxHealthChanged,
	float, NewMaxHP,
	float, CurrentHP
);


UCLASS( ClassGroup=(Combat), meta=(BlueprintSpawnableComponent) )
class DAWNLIGHTREALM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHP = 100.f;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHit OnHit;

	UPROPERTY(EditAnywhere)
	float HitReactThreshold = 15.f; // 伤害超过阈值播放受到攻击动画

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float Damage, AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	void Heal(float Amount); // 改变CurrentHP

	UFUNCTION(BlueprintCallable)  // 改变MaxHP
	void UperLimit(float Amount);

	bool IsDead() const { return CurrentHP <= 0.f; }



};
