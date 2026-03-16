// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBased.h"
#include "Component/HealthComponent.h"

// Sets default values
ACharacterBased::ACharacterBased()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComp = CreateDefaultSubobject<UHealthComponent>("HealthComp");

}

// Called when the game starts or when spawned
void ACharacterBased::BeginPlay()
{
	Super::BeginPlay();
	if (HealthComp)
	{
		HealthComp->OnDeath.AddDynamic(this, &ACharacterBased::Die);
	}
}

// Called every frame
void ACharacterBased::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ACharacterBased::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void ACharacterBased::Die()
{
	SetLifeSpan(3.f);
}

void ACharacterBased::NormalAttack()
{

}


