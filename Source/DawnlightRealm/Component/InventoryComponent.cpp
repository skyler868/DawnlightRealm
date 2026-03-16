// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Items/ItemBase.h"
#include "../PlayerCharacterBased.h"



// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MaxSlots = 9;
	Items.SetNum(MaxSlots);
	// ...
}

bool UInventoryComponent::AddItem(UItemBase* Item)
{
	if (!Item) return false;

	if (Item->bStackable) { // 可堆叠且背包有
		for (UItemBase* CurItem : Items) {
			if (!CurItem) continue;
			if (Item->ItemID == CurItem->ItemID && CurItem->Count <= CurItem->MaxStack) {
				CurItem->Count += Item->Count;
				OnInventoryChanged.Broadcast(); // 数量变化刷新 UI
				return true;
			}
		}
	}
	 
	for (int32 i = 0; i < Items.Num(); i++) { // 不可堆叠或背包没有
		if (Items[i] == nullptr) {
			Items[i] = Item;
			OnInventoryChanged.Broadcast(); // 数量变化刷新 UI
			return true;
		}
	}
	
	return false;
	
}

void UInventoryComponent::UseItem(int32 SlotIndex)
{
	if (!Items.IsValidIndex(SlotIndex)) return;

	UItemBase* Item = Items[SlotIndex];
	if (!Item) return;

	APlayerCharacterBased* Player = Cast<APlayerCharacterBased>(GetOwner());
	if (!Player) return;

	Item->Use(Player);

	if (Item->bConsumable)
	{
		Item->Count--;

		if (Item->Count <= 0)
		{
			RemoveItem(SlotIndex);
		}
	
		OnInventoryChanged.Broadcast(); // 变化刷新 UI
		
	}

}

bool UInventoryComponent::RemoveItem(int32 SlotIndex)
{
	if (!Items.IsValidIndex(SlotIndex)) return false;
	if (!Items[SlotIndex]) return false;

	Items[SlotIndex] = nullptr;
	OnInventoryChanged.Broadcast(); // 变化刷新 UI

	return true;
}

void UInventoryComponent::SwapItems(int32 A, int32 B)
{
	if (!Items.IsValidIndex(A) || !Items.IsValidIndex(B)) return;
	if (A == B) return;
	UItemBase* Temp = Items[A];
	Items[A] = Items[B];
	Items[B] = Temp;

	OnInventoryChanged.Broadcast(); // 变化刷新 UI
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

