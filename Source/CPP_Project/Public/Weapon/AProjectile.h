// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AProjectile.generated.h"

UCLASS()
class CPP_PROJECT_API AAProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAProjectile();

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Shot(const FVector & Direction);

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

protected:

	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY()
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY()
	USphereComponent* SphereCol;

	float LifeTime;
	float CurrentTime;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
