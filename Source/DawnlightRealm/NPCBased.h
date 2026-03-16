// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBased.h"
#include "Interactable.h"
#include "NPCBased.generated.h"

UENUM(BlueprintType)
enum class EDialogueSpeaker : uint8
{
	NPC,
	Player,
	System
};


USTRUCT(BlueprintType)
struct FDialogueOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText OptionText; // UI 上显示的文字 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NextNodeIndex = -1;  // -1 表示结束 点了之后，跳到哪一句 NPC 话

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAcceptQuest = false; //是否接收任务

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCompleteQuest = false; // 是否完成任务
};


USTRUCT(BlueprintType)
struct FDialogueNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDialogueSpeaker Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText NPCText;   // 当前 NPC 显示的对白

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueOption> Options; // 玩家能选的所有选项


};


UCLASS()
class DAWNLIGHTREALM_API ANPCBased : public ACharacterBased, public IInteractable
{
	GENERATED_BODY()
public:
	ANPCBased();

	virtual void Interact_Implementation(AActor* Interactor) override;
protected:
	virtual void Die() override {}
	virtual void NormalAttack() override {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FDialogueNode> DialogueNodes;  // 一个数组 = 一个对话树

	int32 CurrentNodeIndex = 0;

	void StartDialogue();
	UFUNCTION(BlueprintCallable)
	void SelectOption(int32 OptionIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowDialogue(const FDialogueNode& Node);

	UFUNCTION(BlueprintImplementableEvent)
	void EndDialogue();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FName QuestID; // 要自己配置哦 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 InProgressDialogIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 CompletedDialogIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 EverydayDialogIndex;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 NotQuestDialogIndex;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Profile")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Profile")
	UTexture2D* Portrait = nullptr;
	
	UFUNCTION(BlueprintImplementableEvent)
	void GiveReward(FQuestReward reward);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSystemMessage(); // 完成任务UI

	UFUNCTION(BlueprintImplementableEvent)
	void ShowReceiveMessage(); // 接取任务UI

};
