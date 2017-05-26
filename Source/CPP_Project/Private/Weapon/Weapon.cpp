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
}

void UWeapon::Initialize(APawn* pOwnerPawn, AActor* GetHUD)
{
	OwnerPawn = pOwnerPawn;
	CurrentAmmo = MaxAmmo;
	HUD = Cast<AProjectHUD>(GetHUD);
}

void UWeapon::UpdateWeapon(float DeltaSeconds)
{
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
			ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OwnerPawn);
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
			if (Projectile) {
				Projectile->Shot(ProjectileDir);
			}
		}
	}
}

void UWeapon::StartFire()
{
	if (CurrentWeaponState != EMyWeaponState::Fire)
	{
		CurrentWeaponState = EMyWeaponState::Fire;
	}
}

void UWeapon::StopFire()
{
	if (CurrentWeaponState == EMyWeaponState::Fire)
	{
		CurrentWeaponState = EMyWeaponState::Idle;
	}
}
void UWeapon::StartReload()
{
	if (CurrentWeaponState == EMyWeaponState::Reload)
	{
		CurrentAmmo = MaxAmmo;
		ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OwnerPawn);
		Character->AnimReload = false;
		CurrentWeaponState = EMyWeaponState::Idle;
	}
}

int UWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

EMyWeaponState::Type UWeapon::GetWeaponState() const
{
	return EMyWeaponState::Type();
}

void UWeapon::SetWeaponState(EMyWeaponState::Type GetState)
{
	CurrentWeaponState = GetState;
}
