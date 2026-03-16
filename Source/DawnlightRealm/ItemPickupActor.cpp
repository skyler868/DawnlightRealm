// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickupActor.h"
#include "PlayerCharacterBased.h"
#include "Items/ItemBase.h"
#include "Component/InventoryComponent.h"

// Sets default values
AItemPickupActor::AItemPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemPickupActor::Interact_Implementation(AActor* Interactor) // Ìí¼Óµ½±³°ü
{
	APlayerCharacterBased* Player = Cast<APlayerCharacterBased>(Interactor);
	if (!Player) return;

	if (!ItemClass) return;
 	UItemBase* NewItem = NewObject<UItemBase>(Player, ItemClass);
	Player->InventoryComponent->AddItem(NewItem);

	Destroy();

}

