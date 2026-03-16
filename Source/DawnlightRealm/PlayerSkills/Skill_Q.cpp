// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Q.h"
#include "../PlayerCharacterBased.h"
#include "NiagaraFunctionLibrary.h" // 确保已在项目的Build.cs文件中添加了Niagara依赖
#include "NiagaraComponent.h"
#include "Animation/AnimInstance.h"


USkill_Q::USkill_Q()
{
	Cooldown = 18.f; // 冷却时间
	MPCost = 8.f; // MP耗蓝
	bUnlocked = true; // 是否解锁
}

void USkill_Q::Activate(APlayerCharacterBased* Owner)
{
	if (!Owner) return;

    Owner->GetWorldTimerManager().ClearTimer(BuffTimer); // 先清掉旧 Timer

    // 加buff
	Owner->AttackMultiplier *= AttackBuffMultiplier;

    CachedOwner = Owner;

    // 播放动画
	if (QMontage) {
		Owner->PlayAnimMontage(QMontage);
	}

    // 播放特效
    if (QEffect)
    {
        RightHandEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(  //SpawnedEffect = 
            QEffect,
            Owner->GetMesh(),
            FName("hand_r"),
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );

        LeftHandEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
            QEffect,
            Owner->GetMesh(),
            FName("hand_l"),  // 左手骨骼
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
    }

    
    Owner->GetWorldTimerManager().SetTimer(
        BuffTimer,
        //[this, Owner]() {
        //if (Owner) {
        //    Owner->AttackMultiplier /= AttackBuffMultiplier; // 变回去
        //    }
        //// 结束特效
        //auto StopEffect = [](UNiagaraComponent* Effect) {
        //    if (Effect) {
        //        Effect->Deactivate();
        //        Effect->DestroyComponent();
        //        Effect = nullptr;
        //    }
        //};
        //StopEffect(RightHandEffect);
        //StopEffect(LeftHandEffect);
        // 
        //},
        this,
        & USkill_Q::EndBuff,
        BuffDuration,
        false
    );

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Skill_Q, MP - %f, Attack: %f"), MPCost, Owner->GetFinalAttack()));
}

void USkill_Q::EndBuff()
{
    if (!IsValid(CachedOwner)) return;

    CachedOwner->AttackMultiplier /= AttackBuffMultiplier;

    if (RightHandEffect)
    {
        RightHandEffect->Deactivate();
        RightHandEffect->DestroyComponent();
        RightHandEffect = nullptr;
    }

    if (LeftHandEffect)
    {
        LeftHandEffect->Deactivate();
        LeftHandEffect->DestroyComponent();
        LeftHandEffect = nullptr;
    }
}