// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonBoss.h"
#include "../Component/HealthComponent.h"
#include <NiagaraFunctionLibrary.h>
#include "NiagaraComponent.h"
#include "../PlayerCharacterBased.h"
#include "Engine/OverlapResult.h"
#include <Kismet/GameplayStatics.h>


ADragonBoss::ADragonBoss()
{
	AttackType = EEnemyAttackType::Boss;
	EnemyID = "DragonBoss";
	bIsFlying = false;
}

void ADragonBoss::BeginPlay()
{
	Super::BeginPlay();
	InitBossMaterial();
	EnterPhase1(); // 明确起始阶段
}

void ADragonBoss::InitBossMaterial()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	int32 MatCount = MeshComp->GetNumMaterials();

	for (int32 i = 0; i < MatCount; i++)
	{
		UMaterialInstanceDynamic* MID =
			MeshComp->CreateAndSetMaterialInstanceDynamic(i);

		if (MID)
		{
			BossMIDs.Add(MID);
		}
	}
}


void ADragonBoss::CheckPhaseChange()
{
	if (!HealthComp) return;

	float HPPercent = HealthComp->CurrentHP / HealthComp->MaxHP;

	if (HPPercent <= 0.3f && CurrentPhase != EBossPhase::Phase3) {
		EnterPhase3();
	}
	else if (HPPercent <= 0.6f && HPPercent > 0.3f && CurrentPhase != EBossPhase::Phase2) {
		EnterPhase2();
	}

	
}


void ADragonBoss::EnterPhase1()
{
	CurrentPhase = EBossPhase::Phase1;
	AttackCooldown = 1.8f;
	AttackRange = 400.f;
	SetGlow(0.f, FLinearColor::Black);
}

void ADragonBoss::EnterPhase2()
{
	CurrentPhase = EBossPhase::Phase2;
	AttackCooldown = 2.5f;
	AttackRange = 600.f;
	SetGlow(20.f, FLinearColor(1.f, 0.2f, 0.1f));
	OnPhaseChanged.Broadcast(CurrentPhase);
}

void ADragonBoss::EnterPhase3()
{
	CurrentPhase = EBossPhase::Phase3;
	AttackCooldown = 2.4f;
	AttackRange = 700.f;
	SetGlow(8.f, FLinearColor(1.f, 0.2f, 0.f));
	OnPhaseChanged.Broadcast(CurrentPhase);
}


void ADragonBoss::ChooseAttack()
{
	switch (CurrentPhase)
	{
	case EBossPhase::Phase1:
		CurrentAttack = FMath::RandBool()
			? EDragonAttack::Claw
			: EDragonAttack::Bite;
		break;

	case EBossPhase::Phase2:
		CurrentAttack = EDragonAttack::FireBreath;
		break;

	case EBossPhase::Phase3:
		CurrentAttack = FMath::RandBool()
			? EDragonAttack::LightningBall
			: EDragonAttack::FireBreath;
		break;
	}
}

void ADragonBoss::DoBossAttack()
{
	

	CheckPhaseChange();
	ChooseAttack();
	bIsAttacking = true;
	switch (CurrentAttack)
	{
	case EDragonAttack::Claw: BP_PlayClaw(); break;
	case EDragonAttack::Bite: BP_PlayBite(); break;
	case EDragonAttack::FireBreath: BP_PlayFireBreath(); break;

	case EDragonAttack::LightningBall: BP_PlayLightningBall(); break;

	}
}

void ADragonBoss::Die()
{
	Super::Die();

	APlayerCharacterBased* Player =
		Cast<APlayerCharacterBased>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player)
	{
		Player->BP_ShowVictoryUI();
	}
}

void ADragonBoss::SetGlow(float Intensity, FLinearColor Color)
{
	for (UMaterialInstanceDynamic* MID : BossMIDs)
	{
		if (!MID) continue;

		MID->SetScalarParameterValue("EmissiveIntensity", Intensity);
		MID->SetVectorParameterValue("GlowColor", Color);
	}

	
}

void ADragonBoss::ApplyLightBeamDamage(const FVector& Start, const FVector& End)
{
	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Pawn,
		Params
	);


	if (!bHit) return;

	AActor* HitActor = Hit.GetActor();
	if (!HitActor) return;

	
	// 只打玩家
	APlayerCharacterBased* Player =
		Cast<APlayerCharacterBased>(HitActor);

	if (!Player) {
		
		return;
	}

	// 扣血
	if (UHealthComponent* Health = Player->HealthComp)
	{
		Health->ApplyDamage(LightBeamDamage, this);
	}
}

void ADragonBoss::StartLightBeam()
{
	// 防止重复生成
	if (LightBeamComp || !LightBeamSystem) return;

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	// 生成 Niagara
	LightBeamComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		LightBeamSystem,
		MeshComp,
		LightBeamSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true
	);

	if (!LightBeamComp) return;

	// —— 设置 Beam 参数（重点）——
	GetWorld()->GetTimerManager().SetTimer(
		BeamDamageTimer,
		this,
		&ADragonBoss::UpdateLightBeamTarget,
		LightBeamDamageInterval,
		true
	);
}

void ADragonBoss::UpdateLightBeamTarget()
{
	if (!LightBeamComp) return;


	
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	const FTransform SocketTM =
		MeshComp->GetSocketTransform(LightBeamSocketName, RTS_World);

	// 起点：嘴巴位置（高度/位置正确）
	const FVector Start = SocketTM.GetLocation();

	// 方向：Actor 面朝方向（稳定、动画不影响）
	const FVector ForwardDir = MeshComp->GetRightVector();//SocketTM.GetRotation().GetRightVector(); 

	// 终点：直线射出去
	const float BeamLength = 5000.f;
	const FVector End = Start + ForwardDir * BeamLength;

	LightBeamComp->SetVariableVec3(TEXT("User.Start"), Start);
	LightBeamComp->SetVariableVec3(TEXT("User.End"), End);
	
	ApplyLightBeamDamage(Start, End);
}

void ADragonBoss::EndLightBeam()
{
	if (!LightBeamComp) return;
	GetWorld()->GetTimerManager().ClearTimer(BeamDamageTimer);
	LightBeamComp->Deactivate();
	LightBeamComp->DestroyComponent();
	LightBeamComp = nullptr;
}


void ADragonBoss::ApplyFireBreathDamage()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	const FVector Start =
		MeshComp->GetSocketLocation(LightBeamSocketName);

	TArray<FOverlapResult> Hits;
	FCollisionShape Sphere =
		FCollisionShape::MakeSphere(FireBreathRadius);



	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	/*DrawDebugSphere(
		GetWorld(),
		Start,
		FireBreathRadius,
		16,
		FColor::Red,
		false,
		2.f
	);*/

	bool bHit = GetWorld()->OverlapMultiByChannel(
		Hits,
		Start,
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		Params
	);

	if (!bHit) return;
	

	for (auto& Hit : Hits)
	{

		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActor == this) continue;
		
		// 只打玩家
		APlayerCharacterBased* Player =
			Cast<APlayerCharacterBased>(HitActor);

		if (!Player) {

			continue;
		}
		
		// 扣血
		if (UHealthComponent* Health = Player->HealthComp)
		{
			
			Health->ApplyDamage(FireBreathDamage, this);
		}
		

		
	}
}

void ADragonBoss::StartFireBreath()
{
	if (FireBreathComp || !FireBreathSystem) return;

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	FireBreathComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		FireBreathSystem,
		MeshComp,
		LightBeamSocketName,   // 嘴巴 Socket
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);

	GetWorld()->GetTimerManager().SetTimer(
		FireBreathTimer,
		this,
		&ADragonBoss::ApplyFireBreathDamage,
		FireBreathInterval,
		true
	);
}

void ADragonBoss::EndFireBreath()
{
	GetWorld()->GetTimerManager().ClearTimer(FireBreathTimer);

	if (FireBreathComp)
	{
		FireBreathComp->Deactivate();
		FireBreathComp->DestroyComponent();
		FireBreathComp = nullptr;
	}
}

void ADragonBoss::FinishAttack()
{
	bIsAttacking = false;
}
