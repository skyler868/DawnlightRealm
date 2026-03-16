// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBased.h"
#include "WeaponBase.h"

#include "PlayerCharacterBased.generated.h"

class UAnimMontage;
class UPlayerQuestComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMPChanged, 
	float, CurrentMP, float, MaxMP);

UCLASS()
class DAWNLIGHTREALM_API APlayerCharacterBased : public ACharacterBased
{
	GENERATED_BODY()
	
protected:
	

	virtual void BeginPlay() override;

	// 技能管理
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkillManagerComponent* SkillManager;

	//基础属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float MaxMP = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float CurrentMP = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float HPRegenPerSec = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float MPRegenPerSec = 0.f;

public:
	UFUNCTION()
	void PlayHitReact();

	UPROPERTY(EditAnywhere, Category = "Anim")
	UAnimMontage* HitReactMontage;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ShowFailUI();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ShowVictoryUI();
	

	UPROPERTY(EditAnywhere, Category = "Anim")
	UAnimMontage* DeathMontage;


	UFUNCTION()
	virtual void Die() override;

	APlayerCharacterBased();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	// 获取属性
	UPROPERTY(BlueprintAssignable, Category = "Status")
	FOnMPChanged OnMPChanged;

	float GetCurrentMP() const { return CurrentMP; }


	UFUNCTION(BlueprintCallable)
	void AddCurrentMP(float DeltaMP)
	{
		CurrentMP = FMath::Clamp(CurrentMP + DeltaMP, 0.f, MaxMP);
		OnMPChanged.Broadcast(CurrentMP, MaxMP);
	}

	UFUNCTION(BlueprintCallable)
	void AddMaxMP(float ChangeMP) { MaxMP = MaxMP + ChangeMP; 
	   OnMPChanged.Broadcast(CurrentMP, MaxMP);
	}

	float GetFinalAttack() const {
		float FinalAttack = BaseAttack;

		if (IsValid(CurrentWeapon))
		{
			FinalAttack += CurrentWeapon->GetAttack();
		}

		return FinalAttack * AttackMultiplier;
	}

	TObjectPtr<AWeaponBase> GetCurrentWeapon() const { return CurrentWeapon; }
	// 面板
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Profile")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Profile")
	UTexture2D* Portrait = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statue")
	float MaxRunSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statue")
	float MaxWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseAttack = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackMultiplier = 1.f;

	void RegenStatus(float DeltaTime);


	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPlayerQuestComponent* QuestComponent;

	

	// 动画
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* ChargeAttackMontage;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);


	// 攻击相关
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttacking; // 是否在攻击

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 AttackCount = 0;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxAttackCount = 3;

	FTimerHandle AttackResetTimer;

	
	bool bCanCombo;  // 是否在可连招窗口
	FTimerHandle ComboResetTimer; // 超时计时
	bool bRequestNextCombo; // 记录玩家是否在窗口期按了键


	UFUNCTION()
	void ResetCombo();

	UFUNCTION(BlueprintCallable)
	void OnComboEnable();

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ComboResetTime = 2.0f; // 连招超时时间



	// 蓄力攻击 和 攻击
	virtual void StartChargeAttack();
	virtual void ChargeAttack();
	virtual void EndChargeAttack();
	void ExecuteNormalAttack();

	// ===== Attack / Charge 仲裁 =====
	bool bIsCharging = false;

	float ChargeHoldTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ChargeThreshold = 0.10f;


	// 技能
	virtual void Skill_Q();
	virtual void Skill_E();
	virtual void Skill_R();

	// 装备
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AWeaponBase> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AWeaponBase> DefaultWeapon; // 一开始的武器蓝图

	void PickupWeapon(AWeaponBase* Weapon);

	// 移动
	virtual void Move(const FVector2D &Input);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLockMovement = false;

	// 闪避
	UPROPERTY(BlueprintReadOnly, Category = "Dodge")
	FVector2D DodgeInput;   // X = Right, Y = Forward


	UPROPERTY(BlueprintReadOnly, Category = "Dodge")
    bool bIsDodging = false;

	virtual void Dodge();

	UFUNCTION(BlueprintCallable)
	virtual void EndDodge();

	void InterruptCurrentAction();
	
	void RestoreCapsuleCollision();

	// 完美闪避
	//UPROPERTY(BlueprintReadOnly, Category = "Dodge")
	//bool bPerfectDodgeWindow = false;   // 是否处于完美闪避判定窗口

	//UPROPERTY(BlueprintReadOnly, Category = "Dodge")
	//bool bPerfectDodgeSuccess = false;  // 是否已经成功触发

	//UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	//float PerfectDodgeWindowTime = 0.25f;   // 判定窗口时长

	//void TriggerPerfectDodge();

	//// 子弹时间 屏幕黑白变化
	//UPROPERTY(EditDefaultsOnly, Category = "BulletTime")
	//float BulletTimeScale = 0.2f;

	//UPROPERTY(EditDefaultsOnly, Category = "BulletTime")
	//float BulletTimeDuration = 0.25f;

	//FTimerHandle BulletTimeTimer;

	//UFUNCTION(BlueprintCallable)
	//void StartBulletTime();

	//UFUNCTION()
	//void StopBulletTime();

	//UPROPERTY(EditDefaultsOnly, Category = "PostProcess")
	//class UMaterialInterface* PerfectDodgePostProcessMat = nullptr;

	//UPROPERTY()
	//class UMaterialInstanceDynamic* PerfectDodgeMID = nullptr;

	
	// 锁敌
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOn")
	class ULockOnComponent* LockOnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOn")
	bool bIsLockEnemy = false;

	UFUNCTION(BlueprintCallable)
	void SetLockEnemy(bool bLock);

	UFUNCTION(BlueprintCallable)
	void ToggleLockOn();
	
};
