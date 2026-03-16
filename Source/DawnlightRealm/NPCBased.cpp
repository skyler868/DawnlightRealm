// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBased.h"
#include "PlayerCharacterBased.h"
#include "Component/PlayerQuestComponent.h"
#include <Kismet/GameplayStatics.h>

ANPCBased::ANPCBased()
{
	PrimaryActorTick.bCanEverTick = false;
	InProgressDialogIndex = 0;
	CompletedDialogIndex = 0;
	EverydayDialogIndex = CompletedDialogIndex;
}

void ANPCBased::Interact_Implementation(AActor* Interactor)
{
	APlayerCharacterBased* Player = Cast<APlayerCharacterBased>(Interactor);
	if (!Player || !Player->QuestComponent) return;

	EQuestState State = Player->QuestComponent->GetQuestState(QuestID); 

	switch (State)
	{
	case EQuestState::NotAccepted:
		CurrentNodeIndex = 0;              // 接任务对话
		break;

	case EQuestState::InProgress:
	
		CurrentNodeIndex = InProgressDialogIndex;
		break;

	case EQuestState::Completed:
		CurrentNodeIndex = EverydayDialogIndex;
		break;

	}
	if (DialogueNodes.IsValidIndex(CurrentNodeIndex)) {
		ShowDialogue(DialogueNodes[CurrentNodeIndex]); // 执行蓝图显示逻辑
	}
}





void ANPCBased::SelectOption(int32 OptionIndex) {
	if (!DialogueNodes.IsValidIndex(CurrentNodeIndex)) return;

	const FDialogueNode& Node = DialogueNodes[CurrentNodeIndex];
	if (!Node.Options.IsValidIndex(OptionIndex)) return;

	const FDialogueOption& Option = Node.Options[OptionIndex];

	UE_LOG(LogTemp, Warning, TEXT("OptionIndex=%d Accept=%d Complete=%d"),
		OptionIndex,
		Option.bAcceptQuest,
		Option.bCompleteQuest);

	/*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("OptionIndex=%d Accept=%d Complete=%d"),
		OptionIndex,
		Option.bAcceptQuest,
		Option.bCompleteQuest));*/

	if (Option.bAcceptQuest) // 接任务
	{
		APlayerCharacterBased* Player =
			Cast<APlayerCharacterBased>(
				UGameplayStatics::GetPlayerPawn(this, 0));

		if (Player && Player->QuestComponent)
		{
			Player->QuestComponent->AcceptQuest(QuestID);
			ShowReceiveMessage();
			
		}
	}
	
	if (Option.bCompleteQuest)
	{
	
		APlayerCharacterBased* Player =
			Cast<APlayerCharacterBased>(
				UGameplayStatics::GetPlayerPawn(this, 0));
		
		if (Player && Player->QuestComponent)
		{
			bool bSuccess =
				Player->QuestComponent->TryCompleteQuest(QuestID);
			
			if (bSuccess)
			{
				const FQuestData& Quest =
					Player->QuestComponent->GetQuestData(QuestID);

				ShowSystemMessage();
			
				GiveReward(Quest.Reward);   //  给奖励
				CurrentNodeIndex = CompletedDialogIndex;
				ShowDialogue(DialogueNodes[CurrentNodeIndex]);
				return;
			}
			else
			{
				CurrentNodeIndex = NotQuestDialogIndex;
				ShowDialogue(DialogueNodes[CurrentNodeIndex]);//  播“还没完成”的对话
				return;
			}
		}
	}

	// 对话
	if (Option.NextNodeIndex >= 0 && DialogueNodes.IsValidIndex(Option.NextNodeIndex)) {
		CurrentNodeIndex = Option.NextNodeIndex;
		ShowDialogue(DialogueNodes[CurrentNodeIndex]);
	}
	else {
		EndDialogue();
	}
}


