// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillManagerComponent.generated.h"

class USkillBase;
class APlayerCharacterBased;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAWNLIGHTREALM_API USkillManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<APlayerCharacterBased> CachedPlayer;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	TMap<FName, TSubclassOf<USkillBase>> SkillsClasses;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	TMap<FName, TObjectPtr<USkillBase>> Skills;

	//void RegisterSkill(FName Name, USkillBase* Skill);
	bool TryUseSkill(FName Name);

	
		
};
