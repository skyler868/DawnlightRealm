// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillManagerComponent.h"
#include "Skill_Q.h"
#include "Skill_E.h"
#include "Skill_R.h"
#include "../PlayerCharacterBased.h"
#include "../Component/HealthComponent.h"

// Sets default values for this component's properties
USkillManagerComponent::USkillManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	/*RegisterSkill("Q", NewObject<USkill_Q>(this));
	RegisterSkill("E", NewObject<USkill_E>(this));
	RegisterSkill("R", NewObject<USkill_R>(this));*/

	CachedPlayer = Cast<APlayerCharacterBased>(GetOwner());

	for (auto& Pair : SkillsClasses)
	{
		if (Pair.Value)
		{
			USkillBase* Skill = NewObject<USkillBase>(this, Pair.Value);
			if (Skill && CachedPlayer)
			{
				Skill->Init(CachedPlayer);   //  ¹Ø¼ü
			}
			Skills.Add(Pair.Key, Skill);
		}
	}
	
}


// Called every frame
void USkillManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

//void USkillManagerComponent::RegisterSkill(FName Name, USkillBase* Skill)
//{
//	Skills.Add(Name, Skill);
//}

bool USkillManagerComponent::TryUseSkill(FName Name)
{
	
	

	if (!IsValid(this))
	{
		UE_LOG(LogTemp, Error, TEXT("Skill object invalid"));
		return false;
	}

	if (!CachedPlayer) {
		UE_LOG(LogTemp, Error, TEXT("SkillManager not PlayerCharacterBased"));
		return false;
	}
	if (CachedPlayer->IsDead()) return false;
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("TryUse failed: World is nullptr"));
		return false;
	}

	if (TObjectPtr<USkillBase>* SkillPtr = Skills.Find(Name))
	{
		if (USkillBase* Skill = SkillPtr->Get())
		{
			return Skill->TryUse(CachedPlayer.Get());
		}
	}
	return false;
}

