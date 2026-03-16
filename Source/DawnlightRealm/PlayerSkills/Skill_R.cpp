// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_R.h"
#include "../PlayerCharacterBased.h"
#include "NiagaraFunctionLibrary.h" // 确保已在项目的Build.cs文件中添加了Niagara依赖
#include "NiagaraComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/OverlapResult.h"
#include "../Enemy/EnemyBased.h"
#include "../Component/HealthComponent.h"

USkill_R::USkill_R()
{
	Cooldown = 6.f; // 冷却时间
	MPCost = 30.f; // MP耗蓝
	bUnlocked = true; // 是否解锁
}

void USkill_R::Activate(APlayerCharacterBased* Owner)
{
	if (!Owner) return;

	FVector Center = Owner->GetActorLocation();

	//TArray<FOverlapResult> Overlaps;
	//FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);

	// 播放动画
	if (QMontage) {
		Owner->PlayAnimMontage(QMontage, 0.8f);
	}

	if (QEffect)
	{
		float VisualScale = Radius / 100.f;

		UNiagaraComponent* NiagaraComp =
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				Owner->GetWorld(),
				QEffect,
				Center,
				FRotator::ZeroRotator,
				FVector(VisualScale),
				true,
				true
			);
	}

	HitActors.Empty();

	// 每 0.1 秒检测
	Owner->GetWorld()->GetTimerManager().SetTimer(
		AoeTimerHandle,
		FTimerDelegate::CreateUObject(this, &USkill_R::AoeTick, Owner),
		0.1f,
		true
	);

	// 1 秒后结束
	Owner->GetWorld()->GetTimerManager().SetTimer(
		AoeEndHandle,
		FTimerDelegate::CreateUObject(this, &USkill_R::EndAoe, Owner),
		1.f,
		false
	);

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Skill_R AOE, MP - %f"), MPCost));
}


void USkill_R::AoeTick(APlayerCharacterBased* InOwner)
{
	if (!InOwner) return; // 修正：APlayerCharacterBased* 用指针判空

	FVector Center = InOwner->GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InOwner);

	bool bHit = InOwner->GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		Shape,
		Params
	);

	if (!bHit) return;

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor) continue;

		if (HitActors.Contains(HitActor)) continue;

		if (AEnemyBased* Enemy = Cast<AEnemyBased>(HitActor))
		{
			HitActors.Add(HitActor);

			Enemy->OnHit(Damage);

			if (UHealthComponent* Health = Enemy->FindComponentByClass<UHealthComponent>())
			{
				Health->ApplyDamage(Damage, CachedOwner);
			}
		}
	}
}


void USkill_R::EndAoe(APlayerCharacterBased* InOwner)
{
	if (!InOwner) return;

	UWorld* World = InOwner->GetWorld();
	if (!World) return;

	World->GetTimerManager().ClearTimer(AoeTimerHandle);
	World->GetTimerManager().ClearTimer(AoeEndHandle);

	HitActors.Empty();
}
