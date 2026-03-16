// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "Skill_Q.generated.h"

/**
 * 
 */
class UAnimMontage;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class DAWNLIGHTREALM_API USkill_Q : public USkillBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float AttackBuffMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float BuffDuration = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* QMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* QEffect = nullptr;

	UPROPERTY()
	UNiagaraComponent* RightHandEffect = nullptr;

	UPROPERTY()
	UNiagaraComponent* LeftHandEffect = nullptr;

	

	//class UNiagaraComponent* SpawnedEffect = nullptr;
	USkill_Q();
	void EndBuff();
	
	virtual void Activate(class APlayerCharacterBased* Owner) override;
private:
	//¶¨Ê±ÒÆ³ýBuffer
	FTimerHandle BuffTimer;
};
