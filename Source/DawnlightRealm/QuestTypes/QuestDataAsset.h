// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestTypes.h"
#include "QuestDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DAWNLIGHTREALM_API UQuestDataAsset : public UDataAsset  // ≈‰÷√ ˝æ›
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FKillQuestObjective> KillObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FQuestReward Reward;
};
