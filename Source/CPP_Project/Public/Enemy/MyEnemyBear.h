// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MyEnemyBear.generated.h"

/* 곰의 상태 열거형 */
UENUM(BlueprintType)
namespace EMyBearState
{
	enum Type
	{
		Idle,
		Move,
		Attack,
		Die,
	};
}

UCLASS()
class CPP_PROJECT_API AMyEnemyBear : public APawn
{
	GENERATED_BODY()

public:
	AMyEnemyBear();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	/* 베어 무브먼트 컴포넌트 */
	UPROPERTY()
	class UBearMovementComponent* BearMoveComp;

	/* 베어 무브먼트 컴포넌트를 부모 클래스로 반환 */
	UFUNCTION()
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMesh;
	/* 몸통 콜리더 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCapsuleComponent* BodyColl;
	/* 아웃라인 및 드랍 아이템 체크 콜러더 */
	//UPROPERTY(EditDefaultsOnly)
	//class USphereComponent* OutlineAreaColl;
	UPROPERTY(EditAnywhere)
	class USphereComponent* OutlineArea;

	/* 이동 속도 및 회전 속도 */
	UPROPERTY(EditAnywhere, Category = Movement)
	float MoveSpeed;
	UPROPERTY(EditAnywhere, Category = Movement)
	float LookSpeed;

	/* 상태변경 거리 제한 변수들 */
	UPROPERTY(EditAnywhere, Category = Distance)
	float NearDistance;
	UPROPERTY(EditAnywhere, Category = Distance)
	float FarDistance;
	UPROPERTY(EditAnywhere, Category = Distance)
	float AttackDistance;

	/* 이동 */
	UPROPERTY(EditAnywhere, Category = WanderVariable)
	float Offset;
	UPROPERTY(EditAnywhere, Category = WanderVariable)
	float Radius;
	UPROPERTY(EditAnywhere, Category = WanderVariable)
	float Rate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestoryTime;

	/* 상태 반환 함수 */
	UFUNCTION(BlueprintCallable, Category = State)
	EMyBearState::Type GetBearState() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
protected:
	/* 상태 */
	EMyBearState::Type BearState;

	float Angular;
	float SetRot;
	float SetOrientation;
	float UpdateRotation;
	float CurrentHp;

	
	/* 속도 */
	FVector Velocity;
	/* 사망 시 오브젝트 없어질 때까지의 딜레이할 타이머 핸들 */
	FTimerHandle DieTimer;

	/* 추격 상태 */
	void SetArrive(float DeltaTime);
	/* 배회 상태 */
	void StateWander(float DeltaTime);
	/* 공격 상태 */
	void SetAttack();
	bool IsAttack();
	/* 이동 방향 반환 */
	FVector GetSteering();
	/* 타겟을 향해 회전 */
	void LookAtTarget(const FVector TargetLocation, float DeltaTime);
	/* 타겟으로 부터의 정면 방향 반환 */
	FVector GetDirection(const float Orientation);
	/* 타겟을 추격하도록 타겟 위치 반환 */
	FVector SetTrace(FVector TargetVec) const;
	/* 데미지 받음 */
	void AddDamage(const float Damage);
	/* 오브젝트 삭제 */
	void SetDestroy();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOutlineOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOurlineOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
