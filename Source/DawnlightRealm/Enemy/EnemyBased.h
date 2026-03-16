// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBased.h"
#include "EnemyBased.generated.h"

class UBehaviorTree;

UENUM(BlueprintType)
enum class EEnemyAttackType : uint8 {
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),
	Elite UMETA(DisplayName = "Elite"),
	Boss UMETA(DisplayName = "Boss")
};


UCLASS()
class DAWNLIGHTREALM_API AEnemyBased : public ACharacterBased
{
	GENERATED_BODY()

public:
	AEnemyBased();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 10.f;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TryAttack();

	// ==== Κάµ½Ή¥»χ=====
	UPROPERTY(EditDefaultsOnly, Category = "BeAttacked")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "BeAttacked")
	float HitCooldown = 0.8f;

	float LastHitTime = -1000.f;

	UFUNCTION(BlueprintCallable, Category = "BeAttacked")
	void OnHit(float Damage);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* LockOnWidget;

	UFUNCTION(BlueprintCallable)
	void ShowLockOnUI();

	UFUNCTION(BlueprintCallable)
	void HideLockOnUI();

protected:
	bool CanAttack() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void DoMeleeAttack();


	void DoRangedAttack();

	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void DoEliteAttack();

	virtual void DoBossAttack();

	UFUNCTION()
	virtual void Die() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnTargetDetected(AActor* Target);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnEnemyDead();

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackCooldown = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	EEnemyAttackType AttackType;

	float LastAttackTime = -1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FName EnemyID;

	
	
};
