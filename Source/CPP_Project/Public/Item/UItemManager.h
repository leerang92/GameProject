// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "BaseItem.h"
#include "UItemManager.generated.h"

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

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddInventoryList(TArray<FItemStruct> GetItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemStruct> GetDropItem() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemStruct> GetInventoryItem() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CutItemValue(FItemStruct GetItem);
	
protected:
	UPROPERTY()
	TArray<FItemStruct> DropItem;
	UPROPERTY()
	TArray<FItemStruct> InventoryItem;
};
