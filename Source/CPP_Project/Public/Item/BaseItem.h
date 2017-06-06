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

	// 설치할 아이템 위치 및 머테리얼 설정
	virtual void SetBuildItem(const FVector GetPosition, bool IsHit);

	// 아이템 설치 여부 확인 불리언 변수 반환
	virtual bool GetBuildCheck() const;

	// 아이템 설치 여부 확인 불리언 변수 설정
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void SetBuildCheck(bool SetBuild);

	// 아이템의 기존 머테리얼 저장
	virtual void SetOriginalMaterial();

	// 아이템 설치
	virtual void OnBuild();

protected:
	// 설치 가능 여부
	bool IsBuild;

	// 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

private:
	// 설정할 머테리얼
	UPROPERTY()
	class UMaterial* SetMaterial;
	// 설치 가능할 때의 녹색 머테리얼
	UPROPERTY()
	class UMaterial* Green_Mat;
	// 설치 불가능할 때의 적색 머테리얼
	UPROPERTY()
	class UMaterial* Red_Mat;
	// 기존 아이템의 머테리얼 저장할 변수
	UPROPERTY()
	class UMaterial* ItemOriginMaterial;
	
};
