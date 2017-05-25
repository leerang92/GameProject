// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "AItem.h"


// Sets default values
AAItem::AAItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Obj_Mesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	//Mesh->SetStaticMesh(Obj_Mesh.Object);
	//Mesh->SetRelativeScale3D(FVector(100.0f, 100.0f, 100.0f));
	//RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh_Box(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	Mesh->SetStaticMesh(Mesh_Box.Object);
	Mesh->SetRelativeScale3D(FVector(1, 1, 1));
}

// Called when the game starts or when spawned
void AAItem::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AAItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

