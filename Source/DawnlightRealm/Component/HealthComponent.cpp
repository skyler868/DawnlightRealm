// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHP = MaxHP; 
	
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ApplyDamage(float Damage, AActor* InstigatorActor)
{
	if (IsDead()) return;


	CurrentHP -= Damage;
	CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);

	OnHealthChanged.Broadcast(CurrentHP);

	// 只有伤害超过阈值才触发受击
	if (Damage >= HitReactThreshold)
	{
		OnHit.Broadcast();
	}


	if (CurrentHP <= 0.f) {
		OnDeath.Broadcast();
	}
}

void UHealthComponent::Heal(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.f, MaxHP);
	OnHealthChanged.Broadcast(CurrentHP);
}

void UHealthComponent::UperLimit(float Amount)
{
	MaxHP = FMath::Max(1.f, MaxHP + Amount);
	CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);
	
	OnMaxHealthChanged.Broadcast(MaxHP, CurrentHP);
	OnHealthChanged.Broadcast(CurrentHP); // UI 一定要刷新
}



