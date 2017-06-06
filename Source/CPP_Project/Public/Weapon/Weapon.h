// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "ProjectHUD.h"
#include "Weapon.generated.h"

UENUM()
namespace EMyWeaponState
{
	enum Type
	{
		Idle,
		Fire,
		Reload,
	};
}

/**
 * 
 */
UCLASS()
class CPP_PROJECT_API UWeapon : public UObject
{
	GENERATED_BODY()
	
public:
	UWeapon();
	void Initialize(APawn* pOwnerPawn, AActor* GetHUD);
	void UpdateWeapon(float DeltaSeconds);
	void ShotProjectile();
	void StartFire(FVector GetCamLocation, FRotator GetCamRotation);
	void StopFire();
	void StartReload();
	void PlayReloadSound();
	EMyWeaponState::Type GetWeaponState() const;
	void SetWeaponState(EMyWeaponState::Type GetState);

	int GetCurrentAmmo() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxAmmo;


protected:
	int CurrentAmmo;
	float NextFire;
	float FireInterval;
	
	EMyWeaponState::Type CurrentWeaponState;
	TSubclassOf <class AAProjectile> ProjectileClass;

	UPROPERTY()
	APawn* OwnerPawn;
	UPROPERTY()
	AProjectHUD* HUD;
	UPROPERTY()
	UAudioComponent* FireAC;

	UPROPERTY()
	USoundCue* FireLoopSound;
	UPROPERTY()
	USoundCue* FinishFireSound;
	UPROPERTY()
	USoundCue* ReloadSound;
	UPROPERTY()
	UParticleSystem* MuzzleFX;
	UPROPERTY()
	UParticleSystemComponent* FireFX;

	FVector CameraLocation;
	FRotator CameraRotation;

	UAudioComponent* PlaySound(USoundCue* GetSound);
};
