// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "Player/CPP_ProjectCharacter.h"
#include "WarriorCharacter.h"
#include "ProjectHUD.h"

AProjectHUD::AProjectHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> TE_Mesh(TEXT("Texture2D'/Game/UI/crosshair.crosshair'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainOb(TEXT("Texture2D'/Game/UI/HUD/HUDMain.HUDMain'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDTextureOb(TEXT("Texture2D'/Game/UI/HUD/HUDAssets02.HUDAssets02'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDInventoryOb(TEXT("Texture2D'/Game/UI/Inventory.Inventory'"));
	static ConstructorHelpers::FObjectFinder<UFont> Ammo_Font(TEXT("Font'/Game/UI/Roboto-Black.Roboto-Black'"));

	HUDMain = HUDMainOb.Object;
	CrossHair = TE_Mesh.Object;
	HUDMainTexture = HUDTextureOb.Object;
	HUDInventory = HUDInventoryOb.Object;
	AmmoFont = Ammo_Font.Object;

	WeaponIcon = UCanvas::MakeIcon(HUDMain, 310, 210, 150, 60);
	HealthBar = UCanvas::MakeIcon(HUDMainTexture, 67, 212, 372, 50);
	HealthBarBg = UCanvas::MakeIcon(HUDMainTexture, 67, 162, 372, 50);
	HealthIcon = UCanvas::MakeIcon(HUDMainTexture, 78, 262, 28, 28);
	Inventory = UCanvas::MakeIcon(HUDInventory, 50, 100, 150, 50);
}

void AProjectHUD::BeginPlay()
{
	
}

void AProjectHUD::DrawHUD()
{
	if (Canvas == nullptr)
	{
		return;
	}

	DrawCrosshair();
	DrawHealth();
	DrawAmmo();
	DrawTime();
}


void AProjectHUD::DrawHealth()
{
	ACPP_ProjectCharacter* MyCharacter = Cast<ACPP_ProjectCharacter>(GetOwningPawn());
	if (MyCharacter) {
		Canvas->SetDrawColor(FColor::White);
		float ScaleUI = Canvas->ClipY / 1080.0f;
		ScaleUI *= 2.0f;
		const float HealthPosX = (Canvas->ClipX - HealthBarBg.UL * ScaleUI) / 2;
		const float HealthPosY = Canvas->ClipY - (20.0f + HealthBarBg.VL) * ScaleUI;
		Canvas->DrawIcon(HealthBarBg, HealthPosX, HealthPosY, ScaleUI);

		const float HealthAmount = FMath::Min(1.0f, MyCharacter->CurrentHP / MyCharacter->GetMaxHP());
		if (HealthAmount > 0.0f) {
			FCanvasTileItem TileItem(FVector2D(HealthPosX, HealthPosY), HealthBar.Texture->Resource,
				FVector2D(HealthBar.UL * HealthAmount * ScaleUI, HealthBar.VL * ScaleUI), FLinearColor::White);

			MakeUV(HealthBar, TileItem.UV0, TileItem.UV1, HealthBar.U, HealthBar.V, HealthBar.UL * HealthAmount, HealthBar.VL);
			TileItem.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(TileItem);
		}
	}
}

void AProjectHUD::DrawCrosshair()
{
	if (CrossHair)
	{
		/* 캔버스의 중앙 위치에 크로스헤어 정의 */
		FVector2D Center(Canvas->ClipX * 0.474f, Canvas->ClipY * 0.3f);
		FVector2D CrossHairPosition(Center.X - (CrossHair->GetSurfaceWidth() * 0.5f),
			Center.Y - (CrossHair->GetSurfaceHeight() * 0.5f));
		FCanvasTileItem TileItem(CrossHairPosition, CrossHair->Resource, FLinearColor::White);

		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

void AProjectHUD::DrawAmmo()
{
	ACPP_ProjectCharacter* MyCharacter = Cast<ACPP_ProjectCharacter>(GetOwningPawn());
	if (MyCharacter) {
		/* 무기 이미지 그리기 */
		float ScaleUI = Canvas->ClipY / 1080.0f;
		// const float WeaponImgPosX = Canvas->ClipX - Canvas->OrgX - (WeaponIcon.UL / 2.0f + 2 * 80.0f) * ScaleUI;
		const float OffsetX = 160.0f;
		const float OffsetY = 200.0f;

		const float WeaponImgPosX = ((Canvas->ClipX - OffsetX) - WeaponIcon.UL * ScaleUI) / 2;
		const float WeaponImgPosY = Canvas->ClipY - Canvas->OrgY - ((WeaponIcon.VL + WeaponIcon.VL) / 2 + OffsetY) * ScaleUI;
		Canvas->DrawIcon(WeaponIcon, WeaponImgPosX, WeaponImgPosY, ScaleUI * 2);

		/* 탄환 갯수 텍스트 그리기 */
		//ScaleUI *= 2.0f;
		
		FString DrawText = FString::Printf(TEXT("%d / %d"), MyCharacter->GetCurrentAmmo(), MyCharacter->GetMaxAmmo());
		//Canvas->SetDrawColor(FColor::White);
		
		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), AmmoFont, FColor::White);
		float SizeX, SizeY;
		float TextScale = 0.57f;
		Canvas->StrLen(AmmoFont, DrawText, SizeX, SizeY);

		const float TextOffsetY = 145.0f;
		const float TextOffsetX = 50.0f;
		const float AmmoPosX = ((Canvas->ClipX - TextOffsetX) + WeaponIcon.UL * ScaleUI) / 2;
		const float AmmoPosY = Canvas->ClipY - Canvas->OrgY - ((WeaponIcon.VL + WeaponIcon.VL) / 2 + TextOffsetY) * ScaleUI;

		ScaleUI *= 5;
		TextItem.Text = FText::FromString(DrawText);
		TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);

		Canvas->DrawItem(TextItem, AmmoPosX, AmmoPosY);
	}
}

void AProjectHUD::DrawTime()
{
	ACPP_ProjectCharacter* MyCharacter = Cast<ACPP_ProjectCharacter>(GetOwningPawn());
	if (MyCharacter) {
		float ScaleUI = Canvas->ClipY / 1080.0f;
		ScaleUI *= 2.0f;

		const float AmmoPosX = (Canvas->ClipX * ScaleUI) / 2;
		const float AmmoPosY = Canvas->ClipY - (25.0f + HealthBarBg.VL) * ScaleUI;
		FString DrawText = FString::Printf(TEXT("%f"), MyCharacter->CurrentTime);

		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), AmmoFont, FColor::White);
		float SizeX, SizeY;
		float TextScale = 0.57f;
		Canvas->StrLen(AmmoFont, DrawText, SizeX, SizeY);

		const float OffsetX = -1300.0f;
		const float OffsetY = 450.0f;
		const float WeaponImgPosX = ((Canvas->ClipX - OffsetX) - WeaponIcon.UL * ScaleUI) / 2;
		const float WeaponImgPosY = Canvas->ClipY - Canvas->OrgY - ((WeaponIcon.VL + WeaponIcon.VL) / 2 + OffsetY) * ScaleUI;

		TextItem.Text = FText::FromString(DrawText);
		TextItem.Scale = FVector2D(TextScale * ScaleUI * 3, TextScale * ScaleUI * 3);
		Canvas->DrawItem(TextItem, WeaponImgPosX, WeaponImgPosY);
	}
}

void AProjectHUD::MakeUV(FCanvasIcon & Icon, FVector2D & UV0, FVector2D & UV1, uint16 U, uint16 V, uint16 UL, uint16 VL)
{
	if (Icon.Texture)
	{
		const float Width = Icon.Texture->GetSurfaceWidth();
		const float Height = Icon.Texture->GetSurfaceHeight();
		UV0 = FVector2D(U / Width, V / Height);
		UV1 = UV0 + FVector2D(UL / Width, VL / Height);
	}
}
