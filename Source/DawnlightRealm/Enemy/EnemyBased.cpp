// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBased.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "../Component/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../PlayerCharacterBased.h"
#include "../Component/PlayerQuestComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/WidgetComponent.h"


AEnemyBased::AEnemyBased()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; //无论这个 Pawn 是关卡里直接放的，
	//还是运行时 Spawn 出来的，都会自动被 AIController 控制
	AttackType = EEnemyAttackType::Melee;
	EnemyID = "Melee";
	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));
	LockOnWidget->SetupAttachment(GetRootComponent());

	LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidget->SetDrawSize(FVector2D(15.f, 15.f));
	LockOnWidget->SetHiddenInGame(true);
}

void AEnemyBased::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComp)
	{
		HealthComp->OnDeath.AddDynamic(this, &AEnemyBased::Die);
	}
}

void AEnemyBased::TryAttack()
{
	if (!CanAttack()) return;

	switch (AttackType)
	{
	case EEnemyAttackType::Melee:
		DoMeleeAttack();
		break;
	case EEnemyAttackType::Range:
		DoRangedAttack();
		break;
	case EEnemyAttackType::Elite:
		DoEliteAttack();
		break;
	case EEnemyAttackType::Boss:
		DoBossAttack();
		break;
	}
	UWorld* world = GetWorld();
	if (!IsValid(world)) return;
	LastAttackTime = GetWorld()->GetTimeSeconds();
}

void AEnemyBased::OnHit(float Damage)
{
	UWorld* World = GetWorld();
	if (!World) return;

	const float Now = World->GetTimeSeconds();

	// 防止连续抖动
	if (Now - LastHitTime < HitCooldown)
		return;

	LastHitTime = Now;

	// 播放受击动画
	if (HitReactMontage && GetMesh())
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			// 轻微打断当前动画（包括攻击）
			Anim->StopAllMontages(0.1f);
			Anim->Montage_Play(HitReactMontage);
		}
	}

	// 可选：轻微停一下 AI（很重要但不复杂）
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		AICon->StopMovement();
	}
}

bool AEnemyBased::CanAttack() const
{
	UWorld* world = GetWorld();
	if (!IsValid(world)) return false;

	const float Now = world->GetTimeSeconds();

	return (Now - LastAttackTime) >= AttackCooldown;
}

void AEnemyBased::DoMeleeAttack_Implementation()
{
	
}

void AEnemyBased::DoRangedAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("RangedAttack"));
}

void AEnemyBased::DoEliteAttack_Implementation()
{
	
}

void AEnemyBased::DoBossAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("BossAttack"));
}

void AEnemyBased::Die()
{
	//  停 AI 行为树
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		AICon->StopMovement();
		AICon->BrainComponent->StopLogic(TEXT("Dead"));
	}

	//  禁止角色移动
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->DisableMovement();
		
	}

	//  禁用碰撞（避免推人）
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//  禁止转向 / Root Motion 干扰
	bUseControllerRotationYaw = false;
	OnEnemyDead();

	APlayerCharacterBased* Player =
		Cast<APlayerCharacterBased>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player && Player->QuestComponent)
	{
		Player->QuestComponent->NotifyEnemyKilled(EnemyID);
	}

	
	

	Super::Die();

	
}


void AEnemyBased::ShowLockOnUI()
{
	
	if (LockOnWidget)
	{
		LockOnWidget->SetHiddenInGame(false);
	}
}

void AEnemyBased::HideLockOnUI()
{
	if (LockOnWidget)
		LockOnWidget->SetHiddenInGame(true);
}

