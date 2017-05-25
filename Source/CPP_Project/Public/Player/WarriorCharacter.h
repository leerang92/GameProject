// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "UItemManager.h"
#include "TimerManager.h"
#include "CharacterState.h"
#include "WarriorCharacter.generated.h"

UCLASS()
class CPP_PROJECT_API AWarriorCharacter : public ACharacter
{
	GENERATED_BODY()

public:	
	AWarriorCharacter();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void GetItemClass(TSubclassOf<ABaseItem> GetItemClass);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/* 이동 속도 변수들 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed)
	float MovementRunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSpeed)
	float MovementWalkSpeed;

	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUItemManager* ItemManager;

	UFUNCTION(BlueprintCallable, Category = "Character")
	EPlayerState GetPlayerState() const;
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetPlayerState(EPlayerState GetState) { CurrentState = GetState; }

protected:
	
	FTimerHandle BuildTimer;
	UPROPERTY()
	ABaseItem* BuildItem;
	// 최대 체력
	float MaxHP;
	EPlayerState CurrentState;

	/* 캐릭터 움직임 구하는 함수 */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void StartAttack();
	void StopAttack();

	void UpdateBuild();
	void OnBuild();

	void GetDamage(float Damage);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// 최대 체력 반환 함수
	float GetMaxHP() const { return MaxHP; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};
