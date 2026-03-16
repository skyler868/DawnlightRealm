// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "Skill_R.generated.h"



class UAnimMontage;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class DAWNLIGHTREALM_API USkill_R : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_R();
	
	virtual void Activate(class APlayerCharacterBased* Owner) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float Radius = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* QMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* QEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> RCameraShake;

	FTimerHandle AoeTimerHandle; // 在一定时间进入AOE范围都有效
	FTimerHandle AoeEndHandle;

	TSet<TWeakObjectPtr<AActor>> HitActors; // 记录已命中的敌人

	void AoeTick(APlayerCharacterBased* CachedOwner);

	void EndAoe(APlayerCharacterBased* CachedOwner);
	
};
