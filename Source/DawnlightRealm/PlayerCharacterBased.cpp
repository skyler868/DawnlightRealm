// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterBased.h"
#include "./PlayerSkills/SkillManagerComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "EnhancedInputComponent.h"
#include "Component/PlayerQuestComponent.h"
#include "Component/InventoryComponent.h"
#include "Component/HealthComponent.h"
#include "Items/WeaponItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Component/LockOnComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"




void APlayerCharacterBased::PlayHitReact()
{
	if (IsDead()) return;
	InterruptCurrentAction();
	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(HitReactMontage);
	}
}



void APlayerCharacterBased::Die()
{
	InterruptCurrentAction();

	GetCharacterMovement()->DisableMovement();

	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(DeathMontage);
	}
	
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(
		Timer,
		this,
		&APlayerCharacterBased::BP_ShowFailUI,
		4.f,
		false
	);

	
}

APlayerCharacterBased::APlayerCharacterBased()
{
	SkillManager = CreateDefaultSubobject<USkillManagerComponent>(TEXT("SkillManager"));
	QuestComponent = CreateDefaultSubobject<UPlayerQuestComponent>(TEXT("QuestComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComponent"));
	//ComboIndex = 0;
	bIsAttacking = false;
	bCanCombo = false;
	bRequestNextCombo = false;
}


void APlayerCharacterBased::BeginPlay()
{
	Super::BeginPlay();

	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance()) {
		AnimInst->OnMontageEnded.AddDynamic(this, &APlayerCharacterBased::OnAttackMontageEnded);
	}
	if (DefaultWeapon) CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(DefaultWeapon);

	if (CurrentWeapon) CurrentWeapon->OnEquip(this);

	if (HealthComp)
	{
		HealthComp->OnDeath.AddDynamic(this, &APlayerCharacterBased::Die);
		HealthComp->OnHit.AddDynamic(this, &APlayerCharacterBased::PlayHitReact);
	}

	SetLockEnemy(false);

	

}

void APlayerCharacterBased::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RegenStatus(DeltaTime);

	
}

void APlayerCharacterBased::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == NormalAttackMontage) {
		bIsAttacking = false;
		bCanCombo = false;
		//ComboIndex = 0;
		GetWorldTimerManager().ClearTimer(ComboResetTimer);
	}
	RestoreCapsuleCollision();
}

void APlayerCharacterBased::ResetCombo()  // 超时 or 攻击结束统一回到第一段
{
	AttackCount = 0;
}


void APlayerCharacterBased::ExecuteNormalAttack() {
	if (!NormalAttackMontage) return; // || bIsAttacking

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (!AnimInst) return;


	bIsAttacking = true;
	//  累加攻击次数
	AttackCount++;
	if (AttackCount > MaxAttackCount) AttackCount = 1;
	AttackCount = FMath::Clamp(AttackCount, 1, MaxAttackCount);

	//  根据次数决定 Section 名字
	FName SectionName = NAME_None;
	switch (AttackCount)
	{
	case 1: SectionName = "Attack1"; break;
	case 2: SectionName = "Attack2"; break;
	case 3: SectionName = "Attack3"; break;
	}

	//  每一次都【重新播放】Montage
	AnimInst->Montage_Play(NormalAttackMontage, 2.5f);
	AnimInst->Montage_JumpToSection(SectionName, NormalAttackMontage);

	// 重置连击超时（蓝图里的 Retriggerable Delay）
	GetWorldTimerManager().ClearTimer(AttackResetTimer);
	GetWorldTimerManager().SetTimer(
		AttackResetTimer,
		this,
		&APlayerCharacterBased::ResetCombo,
		ComboResetTime,
		false
	);
}

// 2. 动画通知：开启连招窗口 (在动画前半部分调用)
void APlayerCharacterBased::OnComboEnable()
{
	bCanCombo = true;
}



void APlayerCharacterBased::StartChargeAttack()
{
	// 一旦开始蓄力，直接否决普攻
	bIsCharging = true;
	bLockMovement = true;
	ChargeHoldTime = 0.f;

	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(ChargeAttackMontage, 1.0f);
		Anim->Montage_JumpToSection("StartCharge", ChargeAttackMontage);
	}
}

void APlayerCharacterBased::ChargeAttack()
{
	if (!bIsCharging) return;

	ChargeHoldTime += GetWorld()->GetDeltaSeconds();


}

void APlayerCharacterBased::EndChargeAttack()
{
	// 没在蓄力，直接返回
	if (!bIsCharging) return;

	bIsCharging = false;

	// 用时间判断
	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{

		// 先强制打断蓄力蒙太奇
		Anim->Montage_Stop(0.05f, ChargeAttackMontage);

		if (ChargeHoldTime < ChargeThreshold)
		{
			// 短按  普攻
			ExecuteNormalAttack();
		}
		else
		{
			float Alpha = FMath::Clamp(
				ChargeHoldTime / 2.0f, // 2.f 蓄满时间
				0.f,
				1.f
			);

			AttackMultiplier = FMath::Lerp(
				1.f,    // 最小倍率
				2.5f,   // 最大倍率
				Alpha
			);
			// 蓄满  播释放段
			Anim->Montage_Play(ChargeAttackMontage, 1.f);
			Anim->Montage_JumpToSection("ReleaseCharge", ChargeAttackMontage);
		}

		
	}
	AttackMultiplier = 1.f;
	bLockMovement = false;
	ChargeHoldTime = 0.f;
	RestoreCapsuleCollision();
}



void APlayerCharacterBased::RegenStatus(float DeltaTime)
{
	
	CurrentMP = FMath::Min(CurrentMP + DeltaTime * HPRegenPerSec, MaxMP);
}


void APlayerCharacterBased::Skill_Q()
{
	if (!SkillManager) return;
	SkillManager->TryUseSkill("Q");
	
}

void APlayerCharacterBased::Skill_E()
{
	if (!SkillManager) return;
	SkillManager->TryUseSkill("E");
}

void APlayerCharacterBased::Skill_R()
{
	if (!SkillManager) return;
	SkillManager->TryUseSkill("R");
	
}



void APlayerCharacterBased::PickupWeapon(AWeaponBase* Weapon)
{
	if (!Weapon || !InventoryComponent) return;

	// 用地上武器的 Class 生成一把新武器
	AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(
		Weapon->GetClass()
	);

	// 销毁地上的那把（它本来就应该消失）
	Weapon->Destroy();

	if (CurrentWeapon) { // 放到背包
		if (CurrentWeapon->WeaponItemClass)
		{
			UWeaponItem* OldItem = NewObject<UWeaponItem>(
				InventoryComponent,
				CurrentWeapon->WeaponItemClass
			);

			if (OldItem)
			{
				InventoryComponent->AddItem(OldItem);
			}
		}

		CurrentWeapon->OnUnEquip();
		
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
	CurrentWeapon = NewWeapon;
	CurrentWeapon->OnEquip(this);

}

void APlayerCharacterBased::Move(const FVector2D& Input) {
	// 如果没有控制器（Controller 为空），直接返回
    // 说明这个 Pawn 没有被玩家或 AI 控制
	if (Controller == nullptr) return;
	if (bLockMovement) return; // 攻击锁移动

	// 获取控制器当前朝向（只取 Yaw，忽略 Pitch 与 Roll）
	// 这样角色移动方向只受“水平旋转”影响，不会因为镜头抬头/低头乱飞
	const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

	// 根据 Yaw 方向，计算世界空间中的前方向（X轴）
	// 通过旋转矩阵把“控制器朝前”转换为一个方向向量
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// 计算世界空间中的右方向（Y轴）
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 根据输入向量（Input.X / Input.Y）添加移动
	// Input.Y = 前后（W / S） → 对应 Forward
	AddMovementInput(Forward, Input.X);

	// Input.X = 左右（A / D） → 对应 Right
	AddMovementInput(Right, Input.Y);
}


void APlayerCharacterBased::Dodge()
{
	if (bIsDodging) return;

	InterruptCurrentAction(); // 所有的动作全打断

	//bIsDodging = true;
	//bLockMovement = true;
	////GetCharacterMovement()->RootMotionSettings.CharacterRootMotionScale = 1.5f;
	//

	//// 闪避方向
	//FVector DodgeDir = GetLastMovementInputVector();
	//if (DodgeDir.IsNearlyZero()) { // 如果没有 向前
	//	DodgeDir = GetActorForwardVector();
	//}

	//InterruptCurrentAction();

	bIsDodging = true;
	bLockMovement = true;


	// 读取输入轴
	FVector LastInput = GetLastMovementInputVector(); // 注意这里的使用

	// 转成本地2D
	float Forward = FVector::DotProduct(LastInput, GetActorForwardVector());
	float Right = FVector::DotProduct(LastInput, GetActorRightVector());

	DodgeInput = FVector2D(Right, Forward);


	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Forward: %f, Right: %f"), Forward, Right));
	// 没输入 → 默认后闪
	if (DodgeInput.IsNearlyZero())
	{
		DodgeInput = FVector2D(0.f, -1.f);
	}

}



void APlayerCharacterBased::EndDodge() // 动画通知调用
{
	bIsDodging = false;
	bLockMovement = false;
	RestoreCapsuleCollision();

}


void APlayerCharacterBased::RestoreCapsuleCollision()
{
	if (UCapsuleComponent* Cap = GetCapsuleComponent())
	{
		Cap->SetCollisionObjectType(ECC_Pawn);
		Cap->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Cap->SetCollisionResponseToAllChannels(ECR_Block);
	}
}


void APlayerCharacterBased::InterruptCurrentAction()
{
	// 停止所有Montage
	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance()) {
		Anim->Montage_Stop(0.05f);
	}

	// 清攻击和技能状态
	bIsAttacking = false;
	bCanCombo = false;
	bRequestNextCombo = false;
	AttackCount = 0;

	bIsCharging = false;
	ChargeHoldTime = 0.f;

	// 解锁移动
	bLockMovement = false;

	// 清除所有Timer
	GetWorldTimerManager().ClearAllTimersForObject(this);


}

bool APlayerCharacterBased::IsDead() const {
	return HealthComp && HealthComp->IsDead(); 
}


void APlayerCharacterBased::SetLockEnemy(bool bLock)
{
	bIsLockEnemy = bLock;

	if (bLock)
	{
		bUseControllerRotationYaw = true;

		if (GetCharacterMovement())
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else
	{
		bUseControllerRotationYaw = false;

		if (GetCharacterMovement())
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
	}
}

void APlayerCharacterBased::ToggleLockOn() {
	if (LockOnComponent)
	{
		LockOnComponent->ToggleLockOn();
	}

}