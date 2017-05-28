// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "UItemManager.h"

UUItemManager::UUItemManager()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> TE_Bullet(TEXT("Texture2D'/Game/UI/ItemImage/Bullet_Item.Bullet_Item'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> TE_Brunch(TEXT("Texture2D'/Game/UI/ItemImage/Brunch_Item.Brunch_Item'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> BP_BasicItem(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Item/BP_CampFire.BP_CampFire'"));

	FItemStruct Item;
	Item.Image = TE_Bullet.Object;
	Item.Name = TEXT("Bullet");
	Item.Weight = 10.0f;
	Item.Damage = 20.0f;
	Item.Amount = 2;
	DropItem.Add(Item);
	
	Item.Name = TEXT("Brunch");
	Item.Image = TE_Brunch.Object;
	Item.Weight = 20.0;
	Item.Damage = 50.0f;
	Item.Amount = 3;
	Item.ItemClass = (UClass*)BP_BasicItem.Object->GeneratedClass;
	DropItem.Add(Item);

	AddInventoryList(DropItem);
}

void UUItemManager::AddInventoryList(TArray<FItemStruct> GetItemData)
{
	for (int32 i = 0; i < GetItemData.Num(); ++i) 
	{
		InventoryItem.Add(GetItemData[i]);
		//for (int32 j = 0; j < InventoryItem.Num(); ++j)
		//{
		//	UE_LOG(LogClass, Warning, TEXT(" GET ITEM "));
		//	if (InventoryItem[j].Name == GetItemData[i].Name)
		//	{
		//		InventoryItem[j].Amount += GetItemData[i].Amount;
		//		break;
		//	}
		//	else
		//	{
		//		
		//	}
		//}
	}
}

TArray<FItemStruct> UUItemManager::GetDropItem() const
{
	return DropItem;
}

TArray<FItemStruct> UUItemManager::GetInventoryItem() const
{
	return InventoryItem;
}

void UUItemManager::MinusItemNumber(FItemStruct GetItem)
{
	// 선택된 아이템의 수량이 0이 아닐 시 반복문을 통해 선택된 아이템과 같은 아이템를 찾아 수량을 삭감
	if (GetItem.Amount > 0) 
	{	
		for (int32 i = 0; i < InventoryItem.Num(); ++i) 
		{
			if (GetItem.Name == InventoryItem[i].Name)
			{
				InventoryItem[i].Amount--;
				// 현재 사용된 아이템의 갯수가 0 이하면 인벤토리 리스트에서 삭제
				if (InventoryItem[i].Amount <= 0) {
					InventoryItem.RemoveAt(i);
				}
			}
		}
	}
}
