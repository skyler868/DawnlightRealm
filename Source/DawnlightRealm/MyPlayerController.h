// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class DAWNLIGHTREALM_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DefaultMappingDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* CombatMappingDefault;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Dodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_LockOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	UInputAction* IA_ChargeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	UInputAction* IA_Interact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	UInputAction* IA_SkillQ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	UInputAction* IA_SkillE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	UInputAction* IA_SkillR;

private:
	void OnJump();

	void OnStopJump();

	void CallMove(const FInputActionValue& value);

	void OnLook(const FInputActionValue& value);



	void CallStartChargeAttack();

	void CallChargeAttack();

	void CallEndChargeAttack();

	void CallDodge();


	void CallSkillQ();

	void CallSkillE();

	void CallSkillR();

	void CallStartRun();

	void CallEndRun();

	void CallLockOn();
};
