// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "BaseItem.h"


// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsBuild = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = Mesh;

	// 녹색, 적색 머테리얼 설정
	static ConstructorHelpers::FObjectFinder<UMaterial> Mat_Green(TEXT("Material'/Game/Material/Build_Mat.Build_Mat'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Mat_Red(TEXT("Material'/Game/Material/Build_Red_Mat.Build_Red_Mat'"));
	Green_Mat = Mat_Green.Object;
	Red_Mat = Mat_Red.Object;
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ABaseItem::SetBuildItem(const FVector GetPosition, bool IsHit)
{
	// 트레이스 충돌 위치에 아이템 객체 위치
	SetActorLocation(GetPosition);
	// 설치 가능 여부 설정
	SetBuildCheck(IsHit);

	// 설치 가능 여부에 따른 머테리얼 변경
	SetMaterial = IsHit ? Green_Mat : Red_Mat;
	Mesh->SetMaterial(0, SetMaterial);
}

bool ABaseItem::GetBuildCheck() const
{
	return IsBuild;
}

void ABaseItem::SetBuildCheck(bool SetBuild)
{
	IsBuild = SetBuild;
}

// 기존 머테리얼 저장
void ABaseItem::SetOriginalMaterial()
{
	ItemOriginMaterial = (UMaterial*)Mesh->GetMaterial(1);
}

// 아이템 설치
void ABaseItem::OnBuild()
{
	Mesh->SetMaterial(0, ItemOriginMaterial);
}

