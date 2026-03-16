// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "../PlayerCharacterBased.h"
#include "../Component/HealthComponent.h"

bool USkillBase::CanUse(APlayerCharacterBased* Owner) const
{
	if (!bUnlocked || !IsValid(Owner)) return false;

	UWorld* World = Owner->GetWorld();
	if (!IsValid(World)) {
		
		return false;
	}

	float Now = World->GetTimeSeconds();
	if (Now - LastUseTime < Cooldown) return false;
 
	return Owner->GetCurrentMP() >= MPCost;
}

bool USkillBase::ConsumeMP(APlayerCharacterBased* Owner)
{
	if (!Owner) return false;
	float CurrentMP = Owner->GetCurrentMP();
	if (Owner->GetCurrentMP() < MPCost) {
		return false;
	}

	Owner->AddCurrentMP(-MPCost);
	return true;
	
}

void USkillBase::Activate(APlayerCharacterBased* Owner)
{
}

USkillBase::USkillBase()
{
	LastUseTime = -BIG_NUMBER;
}

bool USkillBase::TryUse(APlayerCharacterBased* Owner)
{
	

	if (!IsValid(Owner)) return false;
	if (Owner->IsDead()) return false;

	UWorld* World = Owner->GetWorld();

	if (!World || World->bIsTearingDown || World->HasBegunPlay() == false) {
		UE_LOG(LogTemp, Error, TEXT("World invlalid"));
		return false;
	}

	if (!CanUse(Owner)) return false;

	if (!ConsumeMP(Owner)) return false;

	Activate(Owner);
	LastUseTime = World->GetTimeSeconds();

	OnCooldownStart.Broadcast(Cooldown, LastUseTime);
	return true;
}

void USkillBase::Init(APlayerCharacterBased* InOwner)
{
	CachedOwner = InOwner;
}



//普通空指针	0x0000000000000000
//PendingKill / 已释放 UObject	0xffffffffffffffff
//野指针	随机