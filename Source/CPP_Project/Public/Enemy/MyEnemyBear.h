// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MyEnemyBear.generated.h"

/* ���� ���� ������ */
UENUM(BlueprintType)
namespace EBearState
{
	enum Type
	{
		Idle,
		Wander,
		Arrive,
		Attack,
		Hit,
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
	UPROPERTY(EditAnywhere)
	class USphereComponent* OutlineArea;

	/* �̵� �ӵ� �� ȸ�� �ӵ� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestoryTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
protected:
	/* ���� */
	EBearState::Type BearState;

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
	bool IsAttack();
	/* �̵� ���� ��ȯ */
	FVector GetSteering();
	/* Ÿ���� ���� ȸ�� */
	void LookAtTarget(const FVector TargetLocation, float DeltaTime);
	/* Ÿ������ ������ ���� ���� ��ȯ */
	FVector GetDirection(const float Orientation);
	/* Ÿ���� �߰��ϵ��� Ÿ�� ��ġ ��ȯ */
	FVector SetTrace(FVector TargetVec) const;
	/* ������ ���� */
	void AddDamage(const float Damage);
	/* ������Ʈ ���� */
	void SetDestroy();

	int GetRandomValue(const TArray<float>& Range);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOutlineOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOurlineOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	/* ���� ��ȯ �Լ� */
	UFUNCTION(BlueprintCallable, Category = "Bear State")
	EBearState::Type GetBearState() const { return BearState; }

	UFUNCTION(Blueprintcallable, Category = "Bear State")
	void SetBearState(const EBearState::Type& GetState) { BearState = GetState; }
};
