// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "BaseItem.h"
#include "UItemManager.generated.h"

/* 아이템 타입 */
UENUM(BlueprintType)
namespace EItemType
{
	enum Type
	{
		Use,
		Built,
		Basic,
	};
}

/* 아이템 정보 구조체 */
USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	UTexture2D* Image;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	FName Name;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	float Weight;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	float Damage;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	int Amount;
	UPROPERTY(BlueprintReadWrite, Category = "Item")
	TSubclassOf<ABaseItem> ItemClass;

	EItemType::Type ItemType;

	EItemType::Type GetItemType() const
	{
		return ItemType;
	}
};

/**
*
*/
UCLASS()
class CPP_PROJECT_API UUItemManager : public UObject
{
	GENERATED_BODY()

	UUItemManager();

	/* 인벤토리 리스트에 아이템 정보를 추가한다 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddInventoryList(TArray<FItemStruct> GetItemData);

	/* 드랍한 아이템 리스트를 가져온다 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemStruct> GetDropItem() const;

	/* 인벤토리 아이템 리스트를 가져온다 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemStruct> GetInventoryItem() const;

	/* 아이템의 수량을 삭감한다 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void MinusItemNumber(FItemStruct GetItem);
	
protected:
	/* 드랍 아이템들의 정보를 담는 배열 */
	UPROPERTY()
	TArray<FItemStruct> DropItem;

	/* 인벤토리 아이템들의 정보를 담는 배열 */
	UPROPERTY()
	TArray<FItemStruct> InventoryItem;
};
