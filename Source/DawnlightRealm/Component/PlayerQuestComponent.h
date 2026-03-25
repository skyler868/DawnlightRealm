// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../QuestTypes/QuestTypes.h"
#include "PlayerQuestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestUpdated);
class UQuestDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAWNLIGHTREALM_API UPlayerQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 接/查任务
	UFUNCTION(BlueprintCallable)
	void AcceptQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	EQuestState GetQuestState(FName QuestID) const;  // 获得任务状态


	UFUNCTION(BlueprintCallable)
	bool TryCompleteQuest(FName QuestID); // 完成任务

	UFUNCTION(BlueprintCallable)
	const FQuestData& GetQuestData(FName QuestID) const;  

	// 进度
	UFUNCTION(BlueprintCallable)
	void NotifyEnemyKilled(FName EnemyID); // 击杀敌人

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestUpdated;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FQuestData> Quests; // 任务数据

	UFUNCTION(BlueprintCallable)
	void GetAllQuests(TArray<FQuestData>& OutQuests) const; // TMap转换为TArray 给蓝图用

private:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<UQuestDataAsset*> QuestDataAssets;

	bool IsQuestCompleted(FName QuestID) const;  // 检查所有目标是否完成
};
