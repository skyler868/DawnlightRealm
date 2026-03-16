// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBased.h"
#include "DragonBoss.generated.h"

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1,
	Phase2,
	Phase3
};

UENUM(BlueprintType)
enum class EDragonAttack : uint8 {
	Claw,
	Bite,
	FireBreath,
	LightningBall,
	FlyDive
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FBossPhaseChanged,
	EBossPhase, NewPhase
);


UCLASS()
class DAWNLIGHTREALM_API ADragonBoss : public AEnemyBased
{
	GENERATED_BODY()
	
public:
	ADragonBoss();

protected:
	virtual void BeginPlay() override;
	virtual void DoBossAttack() override;
	virtual void Die() override;


	// ==== Phase ====
	UPROPERTY(BlueprintReadOnly, Category = "Boss")
	EBossPhase CurrentPhase = EBossPhase::Phase1;


	void EnterPhase1();
	void EnterPhase2();
	void EnterPhase3();

	void CheckPhaseChange();

	UPROPERTY(BlueprintAssignable, Category = "Boss")
	FBossPhaseChanged OnPhaseChanged;


	// ===== Attack =====
	void ChooseAttack();

	UPROPERTY(BlueprintReadOnly, Category = "Boss")
	EDragonAttack CurrentAttack;

	
	UPROPERTY(BlueprintReadOnly, Category = "Boss")
	bool bIsFlying;
	

	// === 蓝图播放动画 ===
	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayClaw();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayBite();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayFireBreath();


	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayLightningBall();



	// ===== 材质 =====
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> BossMIDs;

	void InitBossMaterial();
	void SetGlow(float Intensity, FLinearColor Color);
public:

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraComponent* LightBeamComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* LightBeamSystem;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	FName LightBeamSocketName = "Beam";

	// Beam 伤害
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	float LightBeamDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	float LightBeamDamageInterval = 0.5f;

	FTimerHandle BeamDamageTimer;

	UFUNCTION(BlueprintCallable)
	void ApplyLightBeamDamage(const FVector& Start, const FVector& End);


	UFUNCTION(BlueprintCallable) 
	void StartLightBeam();

	UFUNCTION(BlueprintCallable)
	void UpdateLightBeamTarget();


	UFUNCTION(BlueprintCallable)
	void EndLightBeam();

	// ========== Fire Breath =============
	UPROPERTY(EditDefaultsOnly, Category = "Boss|FireBreath")
	UNiagaraSystem* FireBreathSystem;

	UPROPERTY()
	UNiagaraComponent* FireBreathComp;

	UPROPERTY(EditDefaultsOnly, Category = "FireBreath")
	float FireBreathRadius = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBreath")
	float FireBreathDamage = 8.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBreath")
	float FireBreathInterval = 0.8f;

	FTimerHandle FireBreathTimer;

	void ApplyFireBreathDamage();

	UFUNCTION(BlueprintCallable)
	void StartFireBreath();

	UFUNCTION(BlueprintCallable)
	void EndFireBreath();

	// 标记是否正在攻击
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsAttacking = false;

	// 供动画通知调用，结束攻击状态
	UFUNCTION(BlueprintCallable)
	void FinishAttack();
};
