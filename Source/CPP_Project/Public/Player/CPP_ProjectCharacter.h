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

	/* 애니메이션 설정 변수들 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationVariable)
	bool AnimCrouch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationVariable)
	bool AnimReload;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationVariable)
	bool AnimFire;

	/* 이동 속도 변수들 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed)
	float MovmentRunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed)
	float MovementWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed)
	float CrouchSpeed;

	/* 무기 탄환 수 변수들 */
	UPROPERTY(BlueprintReadOnly)
	int CurrentMagazin;
	UPROPERTY(BlueprintReadOnly)
	int MaxMagazin;

	// 현재 체력
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
	// 총구 소켓 이름
	FName MuzzleSocketName;
	// 재장전 대기 타이머 핸들
	FTimerHandle ReloadTimer;
	FTimerHandle BuildTimer;
	UPROPERTY()
	ABaseItem* BuildItem;

	// 달리기 여부
	bool bRunning;
	// 최대 체력
	float MaxHP;

	/** 캐릭터 애니메이션 설정 함수들*/
	void OnCrouch();
	void OffCrouch();
	void StartRun();
	void StopRun();
	void StartJump();
	void StopJump();

	/* 캐릭터 움직임 구하는 함수 */
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

	// 현재 봐라보는 방향 반환 함수
	FRotator SetViewRotator() const {
		return WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}
	// 발사체 발사 될 총기 위치 반환 함수
	FVector GetMuzzleSocketLocation() const {
		return WeaponMesh->GetSocketLocation(MuzzleSocketName);
	}
	// 최대 체력 반환 함수
	float GetMaxHP() const { return MaxHP; }

	int GetCurrentAmmo() const { return BasicWeapon->GetCurrentAmmo(); }
	int GetMaxAmmo() const { return BasicWeapon->MaxAmmo; }

};

