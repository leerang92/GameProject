// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"
#include "CharacterState.h"
#include "Weapon.h"
#include "UItemManager.h"
#include "TimerManager.h"
#include "CPP_ProjectCharacter.generated.h"

UCLASS(config=Game)
class ACPP_ProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ACPP_ProjectCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void GetItemClass(TSubclassOf<ABaseItem> GetItemClass);

	void StartFire();
	void StopFire();
	void OnReload();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/* �ִϸ��̼� ���� ������ */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationVariable)
	bool AnimCrouch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationVariable)
	bool AnimReload;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationVariable)
	bool AnimFire;

	/* �̵� �ӵ� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed)
	float MovmentRunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed)
	float MovementWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed)
	float CrouchSpeed;

	/* ���� źȯ �� ������ */
	UPROPERTY(BlueprintReadOnly)
	int CurrentMagazin;
	UPROPERTY(BlueprintReadOnly)
	int MaxMagazin;

	// ���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUItemManager* ItemManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentTime;
	UPROPERTY(BlueprintReadWrite)
	bool IsDrop;

	UPROPERTY()
	UWeapon* BasicWeapon;

protected:

	EPlayerState CurrentState;
	
	UPROPERTY()
	USkeletalMeshComponent* WeaponMesh;
	// �ѱ� ���� �̸�
	FName MuzzleSocketName;
	// ������ ��� Ÿ�̸� �ڵ�
	FTimerHandle ReloadTimer;
	FTimerHandle BuildTimer;
	UPROPERTY()
	ABaseItem* BuildItem;

	// �޸��� ����
	bool bRunning;
	// �ִ� ü��
	float MaxHP;

	/** ĳ���� �ִϸ��̼� ���� �Լ���*/
	void OnCrouch();
	void OffCrouch();
	void StartRun();
	void StopRun();
	void StartJump();
	void StopJump();

	/* ĳ���� ������ ���ϴ� �Լ� */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void UpdateBuild();
	void OnBuild();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "Character")
	EPlayerState  GetPlayerState() const {
		return CurrentState;
	}

	// ���� ���󺸴� ���� ��ȯ �Լ�
	FRotator SetViewRotator() const {
		return WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}
	// �߻�ü �߻� �� �ѱ� ��ġ ��ȯ �Լ�
	FVector GetMuzzleSocketLocation() const {
		return WeaponMesh->GetSocketLocation(MuzzleSocketName);
	}
	// �ִ� ü�� ��ȯ �Լ�
	float GetMaxHP() const { return MaxHP; }

	int GetCurrentAmmo() const { return BasicWeapon->GetCurrentAmmo(); }
	int GetMaxAmmo() const { return BasicWeapon->MaxAmmo; }

};

