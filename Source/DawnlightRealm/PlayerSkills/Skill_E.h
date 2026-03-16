// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "Skill_E.generated.h"


class ASkillProjectileBase;
class UAnimMontage;

UCLASS()
class DAWNLIGHTREALM_API USkill_E : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_E();

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<ASkillProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* EMontage = nullptr;
	

	virtual void Activate(class APlayerCharacterBased* Owner) override;
};
