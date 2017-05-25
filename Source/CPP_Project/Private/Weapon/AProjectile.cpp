// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "AProjectile.h"


// Sets default values
AAProjectile::AAProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeTime = 3.0f;
	CurrentTime = 0.0f;
	Damage = 10.0f;

	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	SphereCol->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	//SphereCol->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCol->InitSphereRadius(3.5f);
	SphereCol->OnComponentBeginOverlap.AddDynamic(this, &AAProjectile::OnOverlapBegin);
	RootComponent = SphereCol;

	ConstructorHelpers::FObjectFinder<UStaticMesh> FOProjectileMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComp"));
	ProjectileMesh->SetStaticMesh(FOProjectileMesh.Object);
	ProjectileMesh->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
	ProjectileMesh->SetupAttachment(SphereCol);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(SphereCol);
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}
// Called every frame
void AAProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;
	if (CurrentTime > LifeTime)
	{
		Destroy();
	}

}

void AAProjectile::Shot(const FVector & Direction)
{
	ProjectileMovement->Velocity = Direction * MoveSpeed;
}


void AAProjectile::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		Destroy();
	}
}


