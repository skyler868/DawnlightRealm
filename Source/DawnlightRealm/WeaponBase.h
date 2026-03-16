// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class APlayerCharacterBased;
class UWeaponItem;

UCLASS(BlueprintType)
class DAWNLIGHTREALM_API AWeaponBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Attack = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bCanInteract = true;

	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UWeaponItem> WeaponItemClass;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> HitBox;

	UFUNCTION()
	void OnHitBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Interact_Implementation(AActor* Interactor) override;

public:

	UFUNCTION(BlueprintCallable)
	void EnableHitBox();

	UFUNCTION(BlueprintCallable)
	void DisableHitBox();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<AActor*> HitActors;

	void InitFromItem(UWeaponItem* Item);

	UFUNCTION(BlueprintCallable)
	void OnEquip(ACharacter* Character);

	UFUNCTION(BlueprintCallable)
	void OnUnEquip();

	UFUNCTION(BlueprintCallable)
	void DropToWorld();

	float GetAttack() const { return Attack; }

};
