// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "CPP_ProjectCharacter.h"
#include "AProjectile.h"
#include "Weapon.h"

UWeapon::UWeapon()
{
	NextFire = 0.0f;
	FireInterval = 0.2f;
	MaxAmmo = 30;

	static ConstructorHelpers::FClassFinder<AAProjectile> Projectile(TEXT("Class'/Script/CPP_Project.AProjectile'"));
	ProjectileClass = Projectile.Class;

	static ConstructorHelpers::FObjectFinder<USoundCue> FireLoopCue(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/AssaultRifle_ShotLoop_Cue.AssaultRifle_ShotLoop_Cue'"));
	FireLoopSound = FireLoopCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> FinishFireCue(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/AssaultRifle_ShotEnd_Cue.AssaultRifle_ShotEnd_Cue'"));
	FinishFireSound = FinishFireCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> ReloadCue(TEXT("SoundCue'/Game/Sounds/Weapon_AssaultRifle/Mono/AssaultRifle_Reload_Cue.AssaultRifle_Reload_Cue'"));
	ReloadSound = ReloadCue.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireFX(TEXT("ParticleSystem'/Game/Effects/ParticleSystems/Weapons/AssaultRifle/Muzzle/P_AssaultRifle_MF.P_AssaultRifle_MF'"));
	MuzzleFX = FireFX.Object;
}

void UWeapon::Initialize(APawn* pOwnerPawn, AActor* GetHUD)
{
	OwnerPawn = pOwnerPawn;
	CurrentAmmo = MaxAmmo;
	HUD = Cast<AProjectHUD>(GetHUD);
}

void UWeapon::UpdateWeapon(float DeltaSeconds)
{
	ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OwnerPawn);
	if (CurrentWeaponState == EMyWeaponState::Fire)
	{
		NextFire -= DeltaSeconds;
		if (NextFire <= 0.0f && CurrentAmmo > 0)
		{
			NextFire += FireInterval;
			CurrentAmmo--;
			ShotProjectile();
		}
		else if (CurrentAmmo <= 0)
		{
			Character->OnReload();
		}
	}
}

void UWeapon::ShotProjectile()
{
	ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OwnerPawn);
	if (Character)
	{
		const FVector MuzzleLocation = Character->GetMuzzleSocketLocation();
		const FVector ZeroDir = Character->GetActorForwardVector();
		const FVector ZeroPoint = (ZeroDir * 500.f) + MuzzleLocation;
		FVector ProjectileDir = (ZeroPoint - MuzzleLocation);
		ProjectileDir.Normalize();

		UWorld* World = Character->GetWorld();
		if (World) {
			AAProjectile* Projectile = World->SpawnActor<AAProjectile>(ProjectileClass, MuzzleLocation, FRotator(0, 0, 0));
			if (Projectile ) {
				Projectile->Shot(ProjectileDir);
				if (FireAC == NULL && FireFX == NULL) {
					FireAC = PlaySound(FireLoopSound);
					FireFX = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Character->WeaponMesh, Character->MuzzleSocketName);
					FireFX->bOwnerNoSee = false;
					FireFX->bOnlyOwnerSee = true;
				}
			}
		}
	}
}

void UWeapon::StartFire(FVector GetCamLocation, FRotator GetCamRotation)
{
	if (CurrentWeaponState != EMyWeaponState::Fire)
	{
		CameraLocation = GetCamLocation;
		CameraRotation = GetCamRotation;
		CurrentWeaponState = EMyWeaponState::Fire;
	}
}

void UWeapon::StopFire()
{
	if (CurrentWeaponState == EMyWeaponState::Fire)
	{
		CurrentWeaponState = EMyWeaponState::Idle;
		if (FireAC)
		{
			FireAC->FadeOut(0.3f, 0.0f);
			FireAC = NULL;
			PlaySound(FinishFireSound);

			FireFX->Deactivate();
			FireFX = NULL;
		}
	}
}

void UWeapon::StartReload()
{
	ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OwnerPawn);
	if (CurrentWeaponState == EMyWeaponState::Reload)
	{
		CurrentAmmo = MaxAmmo;
		Character->AnimReload = false;
		CurrentWeaponState = EMyWeaponState::Idle;
	}
}

void UWeapon::PlayReloadSound()
{
	if (FireAC && FireFX) {
		FireAC->FadeOut(0.1f, 0.0f);
		FireAC = NULL;

		FireFX->Deactivate();
		FireFX = NULL;
	}
	
}

int UWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

UAudioComponent* UWeapon::PlaySound(USoundCue * GetSound)
{
	UAudioComponent* NewAC = NULL;
	ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OwnerPawn);
	if (GetSound && Character)
	{
		NewAC = UGameplayStatics::SpawnSoundAttached(GetSound, Character->GetRootComponent());
	}
	return NewAC;
}

EMyWeaponState::Type UWeapon::GetWeaponState() const
{
	return EMyWeaponState::Type();
}

void UWeapon::SetWeaponState(EMyWeaponState::Type GetState)
{
	CurrentWeaponState = GetState;
}
