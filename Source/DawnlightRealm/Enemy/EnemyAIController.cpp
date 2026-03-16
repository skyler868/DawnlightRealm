// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyBased.h"

AEnemyAIController::AEnemyAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AEnemyBased* Enemy = Cast<AEnemyBased>(InPawn))
	{
		if (Enemy->BehaviorTree)
		{
			UseBlackboard(Enemy->BehaviorTree->BlackboardAsset, BlackboardComp);
			RunBehaviorTree(Enemy->BehaviorTree);
		}
	}
	
}
