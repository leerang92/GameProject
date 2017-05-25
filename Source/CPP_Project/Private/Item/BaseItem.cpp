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

	// ���, ���� ���׸��� ����
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
	// Ʈ���̽� �浹 ��ġ�� ������ ��ü ��ġ
	SetActorLocation(GetPosition);
	// ��ġ ���� ���� ����
	SetBuildCheck(IsHit);

	// ��ġ ���� ���ο� ���� ���׸��� ����
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

// ���� ���׸��� ����
void ABaseItem::SetOriginalMaterial()
{
	ItemOriginMaterial = (UMaterial*)Mesh->GetMaterial(1);
}

// ������ ��ġ
void ABaseItem::OnBuild()
{
	Mesh->SetMaterial(0, ItemOriginMaterial);
}

