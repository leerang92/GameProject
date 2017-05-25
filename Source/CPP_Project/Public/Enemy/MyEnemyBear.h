// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MyEnemyBear.generated.h"

/* ���� ���� ������ */
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

	/* ���� �����Ʈ ������Ʈ */
	UPROPERTY()
	class UBearMovementComponent* BearMoveComp;

	/* ���� �����Ʈ ������Ʈ�� �θ� Ŭ������ ��ȯ */
	UFUNCTION()
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMesh;
	/* ���� �ݸ��� */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCapsuleComponent* BodyColl;
	/* �ƿ����� �� ��� ������ üũ �ݷ��� */
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* OutlineAreaColl;
	/* ���� ���� ���� �ݸ��� */
	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* AttackColl;

	/* �̵� �ӵ� �� ȸ�� �ӵ� */
	UPROPERTY(EditAnywhere, Category = Movement)
	float MoveSpeed;
	UPROPERTY(EditAnywhere, Category = Movement)
	float LookSpeed;

	/* ���º��� �Ÿ� ���� ������ */
	UPROPERTY(EditAnywhere, Category = Distance)
	float NearDistance;
	UPROPERTY(EditAnywhere, Category = Distance)
	float FarDistance;
	UPROPERTY(EditAnywhere, Category = Distance)
	float AttackDistance;

	/* �̵� */
	UPROPERTY(EditAnywhere, Category = WanderVariable)
	float Offset;
	UPROPERTY(EditAnywhere, Category = WanderVariable)
	float Radius;
	UPROPERTY(EditAnywhere, Category = WanderVariable)
	float Rate;

	/* ���� ���� ���� ���� */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestoryTime;

	/* ���� ��ȯ �Լ� */
	UFUNCTION(BlueprintCallable, Category = State)
	EMyBearState::Type GetBearState() const;

protected:
	/* ���� */
	EMyBearState::Type BearState;

	float Angular;
	float SetRot;
	float SetOrientation;
	float UpdateRotation;
	float CurrentHp;
	
	/* �ӵ� */
	FVector Velocity;
	/* ��� �� ������Ʈ ������ �������� �������� Ÿ�̸� �ڵ� */
	FTimerHandle DieTimer;

	/* �߰� ���� */
	void SetArrive(float DeltaTime);
	/* ��ȸ ���� */
	void StateWander(float DeltaTime);
	/* ���� ���� */
	void SetAttack();
	/* �̵� ���� ��ȯ */
	FVector GetSteering();
	/* Ÿ���� ���� ȸ�� */
	void LookAtTarget(FVector TargetLocation, float DeltaTime);
	/* Ÿ������ ������ ���� ���� ��ȯ */
	FVector GetDirection(float Orientation);
	/* Ÿ���� �߰��ϵ��� Ÿ�� ��ġ ��ȯ */
	FVector SetTrace(FVector TargetVec) const;
	/* ������ ���� */
	void AddDamage(float Damage);
	/* ������Ʈ ���� */
	void SetDestroy();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapAttack(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapAttackEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
