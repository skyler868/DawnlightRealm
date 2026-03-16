// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

class APlayerCharacterBased;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnSkillCooldownStart,
	float, CooldownTime,
	float, StartTime
);

UCLASS(Blueprintable, Abstract)
class DAWNLIGHTREALM_API USkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkillBase")
	float Cooldown = 0.f; // 冷却时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillBase")
	float MPCost = 0.f; // MP耗蓝

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillBase")
	bool bUnlocked = true; // 是否解锁

	//int32 Level = 1;   // 等级
	float LastUseTime = -BIG_NUMBER;

	USkillBase();
	bool TryUse(APlayerCharacterBased* Owner);

	void Init(APlayerCharacterBased* InOwner);

	UPROPERTY(BlueprintAssignable)
	FOnSkillCooldownStart OnCooldownStart;

protected:
	UPROPERTY()   //  必须
	TObjectPtr<APlayerCharacterBased> CachedOwner;

	virtual bool CanUse(APlayerCharacterBased* Owner) const;

	
	bool ConsumeMP(APlayerCharacterBased* Owner);

	virtual void Activate(class APlayerCharacterBased* Owner);
};
