// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerQuestComponent.h"
#include "../QuestTypes/QuestDataAsset.h"

// Sets default values for this component's properties
UPlayerQuestComponent::UPlayerQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UPlayerQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerQuestComponent::AcceptQuest(FName QuestID)
{
	if (Quests.Contains(QuestID)) {
		return;
	}

	UQuestDataAsset* Asset = nullptr;

	for (UQuestDataAsset* DA : QuestDataAssets) {
		if (!DA) continue;
		if (DA && DA->QuestID == QuestID) {
			Asset = DA;
			break;
		}
	}

	if (!Asset)
	{
		UE_LOG(LogTemp, Error, TEXT("AcceptQuest failed: QuestDataAsset not found for QuestID: %s"),
			*QuestID.ToString());
		return;
	}
	FQuestData NewQuest;
	NewQuest.QuestID = QuestID;
	NewQuest.State = EQuestState::InProgress;
	NewQuest.KillObjectives = Asset->KillObjectives;
	NewQuest.QuestTitle = Asset->QuestTitle;
	NewQuest.Reward = Asset->Reward;

	Quests.Add(QuestID, NewQuest);
	OnQuestUpdated.Broadcast();
}

EQuestState UPlayerQuestComponent::GetQuestState(FName QuestID) const
{
	if (const FQuestData* Quest = Quests.Find(QuestID)) {
		return Quest->State;
	}

	return EQuestState::NotAccepted;
}

void UPlayerQuestComponent::NotifyEnemyKilled(FName EnemyID)
{
	for (auto& Pair : Quests) {
		FQuestData& Quest = Pair.Value;
		if (Quest.State != EQuestState::InProgress) continue;

		for (FKillQuestObjective& obj : Quest.KillObjectives) {
			if (obj.EnemyID == EnemyID && obj.CurrentCount < obj.RequiredCount) {
				obj.CurrentCount++;
				/*if (IsQuestCompleted(Quest.QuestID)) {
					Quest.State = EQuestState::Completed;
				}*/
				OnQuestUpdated.Broadcast();
			}
		}
	}
}


void UPlayerQuestComponent::GetAllQuests(TArray<FQuestData>& OutQuests) const
{
	OutQuests.Empty();

	for (const auto& Pair : Quests)
	{
		OutQuests.Add(Pair.Value);
	}
}

bool UPlayerQuestComponent::IsQuestCompleted(FName QuestID) const
{
	const FQuestData* Quest = Quests.Find(QuestID);
	if (!Quest) return false;

	for (const FKillQuestObjective& Obj : Quest->KillObjectives)
	{
		if (Obj.CurrentCount < Obj.RequiredCount)
			return false;
	}
	return true;
}

bool UPlayerQuestComponent::TryCompleteQuest(FName QuestID)
{
	FQuestData* Quest = Quests.Find(QuestID);
	if (!Quest) return false;

	if (Quest->State != EQuestState::InProgress ||
		!IsQuestCompleted(QuestID))
	{
		return false;
	}

	Quest->State = EQuestState::Completed;

	OnQuestUpdated.Broadcast();
	return true;
}

const FQuestData& UPlayerQuestComponent::GetQuestData(FName QuestID) const
{
	return Quests.FindChecked(QuestID);
}
