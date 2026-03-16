// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacterBased.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 获取本地玩家的 EnhancedInput 子系统
	if (UEnhancedInputLocalPlayerSubsystem* Sub
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(GetLocalPlayer())) {
		// 把配置好的输入 Mapping Context 加进去
		Sub->AddMappingContext(DefaultMappingDefault, 0);
		Sub->AddMappingContext(CombatMappingDefault, 1);
		
	}
	
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// 把 UE 默认 InputComponent 转成 EnhancedInputComponent
	// 因为增强输入需要用 UEnhancedInputComponent 才能 BindAction
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EIC) return;


	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyPlayerController::CallMove);

	//EIC->BindAction(IA_Attack, ETriggerEvent::Started, this, &AMyPlayerController::CallAttack);

	// 蓄力攻击和攻击
	EIC->BindAction(IA_Attack, ETriggerEvent::Started, this, &AMyPlayerController::CallStartChargeAttack);
	EIC->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AMyPlayerController::CallChargeAttack);
	EIC->BindAction(IA_Attack, ETriggerEvent::Completed, this, &AMyPlayerController::CallEndChargeAttack);
	EIC->BindAction(IA_Attack, ETriggerEvent::Canceled, this, &AMyPlayerController::CallEndChargeAttack);


	// Dodge（闪避）
	EIC->BindAction(IA_Dodge, ETriggerEvent::Triggered, this, &AMyPlayerController::CallDodge);

	// 锁敌
	EIC->BindAction(IA_LockOn, ETriggerEvent::Started, this, &AMyPlayerController::CallLockOn);


	EIC->BindAction(IA_SkillQ, ETriggerEvent::Started, this, &AMyPlayerController::CallSkillQ);
	EIC->BindAction(IA_SkillE, ETriggerEvent::Started, this, &AMyPlayerController::CallSkillE);
	EIC->BindAction(IA_SkillR, ETriggerEvent::Started, this, &AMyPlayerController::CallSkillR);

	EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyPlayerController::OnJump);
	EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AMyPlayerController::OnStopJump);

	EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyPlayerController::OnLook);

	EIC->BindAction(IA_Run, ETriggerEvent::Started, this, &AMyPlayerController::CallStartRun);
	EIC->BindAction(IA_Run, ETriggerEvent::Completed, this, &AMyPlayerController::CallEndRun); //松开按键，且之前达到了 Triggered 才触发
	EIC->BindAction(IA_Run, ETriggerEvent::Canceled, this, &AMyPlayerController::CallEndRun); // 松开按键，但没有达到 Triggered 时触发（= “点按” 就是这种）
}

void AMyPlayerController::OnJump()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->Jump();
	}
}

void AMyPlayerController::OnStopJump()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->StopJumping();
	}
}

void AMyPlayerController::OnLook(const FInputActionValue& value)
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		
		FVector2D LookInput = value.Get<FVector2D>();
		P->AddControllerYawInput(LookInput.X);
		P->AddControllerPitchInput(-LookInput.Y);

	}

}

void AMyPlayerController::CallMove(const FInputActionValue& value)
{
	FVector2D Input = value.Get<FVector2D>();


	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {

		P->Move(Input);
	}

}



// 蓄力攻击
void AMyPlayerController::CallStartChargeAttack()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->StartChargeAttack();
	}
}

void AMyPlayerController::CallChargeAttack()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->ChargeAttack();
	}
}

void AMyPlayerController::CallEndChargeAttack()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->EndChargeAttack();
	}

}



void AMyPlayerController::CallDodge()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {

		P->Dodge();
	/*	if (!P->IsHoldingBack())
			P->Dodge();
		else  P->DodgeBack();*/
	}
}

void AMyPlayerController::CallSkillQ()
{
	
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->Skill_Q();
	}
}

void AMyPlayerController::CallSkillE()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->Skill_E();
	}
}

void AMyPlayerController::CallSkillR()
{

	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->Skill_R();
	}
}

void AMyPlayerController::CallStartRun()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->GetCharacterMovement()->MaxWalkSpeed = P->MaxRunSpeed;
	}
}

void AMyPlayerController::CallEndRun()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->GetCharacterMovement()->MaxWalkSpeed = P->MaxWalkSpeed;
	}
}

void AMyPlayerController::CallLockOn()
{
	if (APlayerCharacterBased* P = Cast<APlayerCharacterBased>(GetPawn())) {
		P->ToggleLockOn();
	}
}



