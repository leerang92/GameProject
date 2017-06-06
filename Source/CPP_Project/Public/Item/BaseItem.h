// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class CPP_PROJECT_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseItem();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	// ��ġ�� ������ ��ġ �� ���׸��� ����
	virtual void SetBuildItem(const FVector GetPosition, bool IsHit);

	// ������ ��ġ ���� Ȯ�� �Ҹ��� ���� ��ȯ
	virtual bool GetBuildCheck() const;

	// ������ ��ġ ���� Ȯ�� �Ҹ��� ���� ����
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void SetBuildCheck(bool SetBuild);

	// �������� ���� ���׸��� ����
	virtual void SetOriginalMaterial();

	// ������ ��ġ
	virtual void OnBuild();

protected:
	// ��ġ ���� ����
	bool IsBuild;

	// �޽� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

private:
	// ������ ���׸���
	UPROPERTY()
	class UMaterial* SetMaterial;
	// ��ġ ������ ���� ��� ���׸���
	UPROPERTY()
	class UMaterial* Green_Mat;
	// ��ġ �Ұ����� ���� ���� ���׸���
	UPROPERTY()
	class UMaterial* Red_Mat;
	// ���� �������� ���׸��� ������ ����
	UPROPERTY()
	class UMaterial* ItemOriginMaterial;
	
};
