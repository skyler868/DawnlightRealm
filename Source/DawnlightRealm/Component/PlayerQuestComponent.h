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
	// 接/查
	UFUNCTION(BlueprintCallable)
	void AcceptQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	EQuestState GetQuestState(FName QuestID) const;


	UFUNCTION(BlueprintCallable)
	bool TryCompleteQuest(FName QuestID);

	UFUNCTION(BlueprintCallable)
	const FQuestData& GetQuestData(FName QuestID) const;

	// 进度
	UFUNCTION(BlueprintCallable)
	void NotifyEnemyKilled(FName EnemyID);

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestUpdated;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FQuestData> Quests;

	UFUNCTION(BlueprintCallable)
	void GetAllQuests(TArray<FQuestData>& OutQuests) const;
private:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<UQuestDataAsset*> QuestDataAssets;

	bool IsQuestCompleted(FName QuestID) const;

	
};
