// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "ProjectHUD.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PROJECT_API AProjectHUD : public AHUD
{
	GENERATED_BODY()

public:
	AProjectHUD();
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	void GetCurrentAmmo(int GetAmmo);

protected:
	UPROPERTY()
	UTexture2D* CrossHair;
	UPROPERTY()
	UTexture2D* HUDMainTexture;
	UPROPERTY()
	UTexture2D* HUDTexture;
	UPROPERTY()
	UTexture2D* HUDInventory;
	UPROPERTY()
	UTexture2D* HUDMain;
	UPROPERTY()
	UFont* AmmoFont;

	UPROPERTY()
	FCanvasIcon HealthBar;
	UPROPERTY()
	FCanvasIcon HealthBarBg;
	UPROPERTY()
	FCanvasIcon HealthIcon;
	UPROPERTY()
	FCanvasIcon Inventory;
	UPROPERTY()
	FCanvasIcon WeaponIcon;

	int CurrentAmmo;

	void DrawHealth();

	void DrawCrosshair();

	void DrawAmmo();

	void DrawTime();

	void MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL);
};
