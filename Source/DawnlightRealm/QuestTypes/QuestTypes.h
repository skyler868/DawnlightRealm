// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.generated.h"

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	NotAccepted,
	InProgress,
	Completed,

};

USTRUCT(BlueprintType)
struct FKillQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EnemyID;          // 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RequiredCount = 1; // 需求数量

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentCount = 0; // 当前数量
};

USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class UItemBase>> RewardItems;
};

USTRUCT(BlueprintType)
struct FQuestData // 运行时数据
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName QuestID;

	UPROPERTY(BlueprintReadOnly)
	EQuestState State = EQuestState::NotAccepted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestTitle;

	UPROPERTY(BlueprintReadOnly)
	TArray<FKillQuestObjective> KillObjectives;

	UPROPERTY(BlueprintReadOnly)
	FQuestReward Reward;

};


class  QuestTypes
{


};
