// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "ItemPickupActor.generated.h"

class UItemBase;

UCLASS()
class DAWNLIGHTREALM_API AItemPickupActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* Interactor) override; // Ìí¼Óµ½±³°ü

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemBase> ItemClass;

};
