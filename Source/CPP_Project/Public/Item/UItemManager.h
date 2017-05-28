// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "BaseItem.h"
#include "UItemManager.generated.h"

/* ������ Ÿ�� */
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

/* ������ ���� ����ü */
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

	/* �κ��丮 ����Ʈ�� ������ ������ �߰��Ѵ� */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddInventoryList(TArray<FItemStruct> GetItemData);

	/* ����� ������ ����Ʈ�� �����´� */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemStruct> GetDropItem() const;

	/* �κ��丮 ������ ����Ʈ�� �����´� */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemStruct> GetInventoryItem() const;

	/* �������� ������ �谨�Ѵ� */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void MinusItemNumber(FItemStruct GetItem);
	
protected:
	/* ��� �����۵��� ������ ��� �迭 */
	UPROPERTY()
	TArray<FItemStruct> DropItem;

	/* �κ��丮 �����۵��� ������ ��� �迭 */
	UPROPERTY()
	TArray<FItemStruct> InventoryItem;
};
