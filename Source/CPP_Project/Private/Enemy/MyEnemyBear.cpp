// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "AProjectile.h"
#include "CPP_ProjectCharacter.h"
#include "BearMovementComponent.h"
#include "MyEnemyBear.h"


// Sets default values
AMyEnemyBear::AMyEnemyBear()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentHp = 100.0f;

	// �߰� ������
	MoveSpeed = 300.0f;
	LookSpeed = 200.0f;
	Angular = 0.0f;
	SetRot = 0.0f;
	SetOrientation = 0.0f;
	UpdateRotation = 0.0f;
	DestoryTime = 15.0f;
	Velocity = FVector::ZeroVector;

	// ��ȸ ������
	Offset = 60.0f;
	Radius = 3.0f;
	Rate = 60.0f;

	// ���� �Ÿ� ������
	NearDistance = 1400.0f;
	FarDistance = 1500.0f;
	AttackDistance = 200.0f;
	BearState = EBearState::Idle;

	Weight = 20.0f;

	// ĸ�� �ø��� ����
	BodyColl = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	BodyColl->SetCapsuleHalfHeight(108.497429f);
	BodyColl->SetCapsuleRadius(108.497429f);
	BodyColl->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemyBear::OnOverlapBegin);
	BodyColl->SetEnableGravity(true);
	//CapsuleColl->SetSimulatePhysics(true);
	RootComponent = BodyColl;

	// �ƿ����� �� ��� ���� ���� �ø��� ����
	OutlineArea = CreateDefaultSubobject<USphereComponent>(TEXT("Outline Area"));
	// 300.0f
	OutlineArea->SetSphereRadius(1000.0f);
	OutlineArea->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemyBear::OnOutlineOverlapBegin);
	OutlineArea->OnComponentEndOverlap.AddDynamic(this, &AMyEnemyBear::OnOurlineOverlapEnd);
	OutlineArea->SetupAttachment(RootComponent);

	// ���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(
		TEXT("SkeletalMesh'/Game/Model/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear.Enemy_Bear'"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bear Skeletal Mesh Componenet"));
	SkeletalMesh->SetRelativeLocationAndRotation(FVector(0, 0, -120.0f), FRotator(0, -90.0f, 0));
	SkeletalMesh->SetSkeletalMesh(SK_Mesh.Object);
	SkeletalMesh->SetupAttachment(BodyColl);

	// �ִϸ��̼� �������Ʈ ����
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> BP_Anim(
		TEXT("AnimBlueprint'/Game/Model/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear_Animations/EnemyBearAnimBP.EnemyBearAnimBP'"));
	SkeletalMesh->SetAnimInstanceClass(BP_Anim.Object->GeneratedClass);

	// �����Ʈ ����
	BearMoveComp = CreateDefaultSubobject<UBearMovementComponent>(TEXT("BearMovementComponent"));
	BearMoveComp->UpdatedComponent = RootComponent;
	BearState = EBearState::Wander;
}

void AMyEnemyBear::BeginPlay()
{
	Super::BeginPlay();
}

void AMyEnemyBear::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (Weight <= 0.0f)
	{
		MoveSpeed = 0.0f;
		BearState = EBearState::Idle;
	}

	switch (BearState)
	{
	case EBearState::Wander:
		Weight -= DeltaTime;
		StateWander(DeltaTime);
		break;
	case EBearState::Arrive:
		SetArrive(DeltaTime);
		break;
	case EBearState::Attack:
		SetAttack();
		break;
	}
}

UPawnMovementComponent * AMyEnemyBear::GetMovementComponent() const
{
	return BearMoveComp;
}

// �÷��̾ �߰��Ͽ� �̵�
FVector AMyEnemyBear::GetSteering()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GWorld, 0);
	FVector Direction = Character->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.0f;
	Direction.Normalize();
	return Direction;
}

void AMyEnemyBear::SetArrive(float DeltaTime)
{
	// ĳ������ ��ġ�� ����.
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GWorld, 0);
	FVector TraceVec = SetTrace(Character->GetActorLocation());
	LookAtTarget(TraceVec, DeltaTime);

	const float Distance = GetDistanceTo(Character);
	if (Distance < 200.0f)
	{
		MoveSpeed = 10.0f;
	}
	else
	{
		MoveSpeed = 300.0f;
	}
	// �������� �̵�
	BearMoveComp->AddInputVector(GetActorForwardVector() * MoveSpeed * DeltaTime);
	if (Distance < 250.0f && IsAttack()) {
		BearState = EBearState::Attack;
	}
}

void AMyEnemyBear::SetAttack()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GWorld, 0);

	const float Distance = GetDistanceTo(Character);
	if (!IsAttack() || Distance > 250.0f) {
		BearState = EBearState::Arrive;
	}
}

bool AMyEnemyBear::IsAttack()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GWorld, 0);;
	//float Dir = GetDotProductTo(Character);
	// ĳ���Ϳ��� ������ ���� (0 ~ 1 ����)
	const FVector ForwardVec = GetActorForwardVector();
	FVector TargetOffset = Character->GetActorLocation() - GetActorLocation();
	TargetOffset = TargetOffset.GetSafeNormal();
	float Angle = FVector::DotProduct(ForwardVec, TargetOffset);

	if (Angle >= 0.8f) { return true; }

	return false;
}

void AMyEnemyBear::LookAtTarget(const FVector TargetLocation, float DeltaTime)
{
	// ĳ���Ϳ��� �Ÿ��� ����
	// Atan2()�� ����Ͽ� Y, X �ι����� ������ ���� 
	FVector Direction = TargetLocation - GetActorLocation();
	float Orientation = FMath::Atan2(Direction.Y, Direction.X);
	Orientation = FMath::RadiansToDegrees(Orientation);

	// ���� ������ ����
	float Rotation = Orientation - SetOrientation;
	Rotation = FMath::Fmod(Rotation, 360.0f);
	if(FMath::Abs(Rotation) > 180.0f)
	{
		if (Rotation < 0.0f)
			Rotation += 360.0f;
		else
			Rotation -= 360.0f;
	}

	float RotationSize = FMath::Abs(Rotation);
	float TargetRotation = LookSpeed * (Rotation / RotationSize);
	Angular = TargetRotation - UpdateRotation;
	Angular /= 0.1f;

	UpdateRotation += Angular * DeltaTime;
	if (UpdateRotation > 45.0f) 
	{
		UpdateRotation = 45.0f;
	}
	if (Angular == 0.0f)
	{
		Angular = 0.0f;
	}

	SetOrientation += UpdateRotation * DeltaTime;
	if (SetOrientation < 0.0f)
	{
		SetOrientation += 360.0f;
	}
	else if (SetOrientation > 360.0f)
	{
		SetOrientation -= 360.0f;
	}

	FRotator MoveRototation = FRotator::ZeroRotator;
	MoveRototation.Yaw = SetOrientation;
	SetActorRotation(MoveRototation);
}

void AMyEnemyBear::StateWander(float DeltaTime)
{
	float WanderOrientation = FMath::RandRange(-1.0f, 1.0f) * Rate;
	FRotator SetOri = GetActorRotation();
	float TargetOrientation = WanderOrientation + SetOrientation;

	FVector OrientationVec = GetDirection(SetOrientation);
	FVector TargetPosition = (Offset * OrientationVec) + GetActorLocation();
	TargetPosition += (GetDirection(TargetOrientation) * Radius);

	TargetPosition = SetTrace(TargetPosition);
	// Ÿ���� ���� ȸ��
	LookAtTarget(TargetPosition, DeltaTime);

	// Ÿ���� ���� �̵�
	FVector Direction = TargetPosition - GetActorLocation();
	Direction.Normalize();
	BearMoveComp->AddInputVector(Direction * MoveSpeed * DeltaTime);
}

FVector AMyEnemyBear::GetDirection(const float Orientation)
{
	// Ÿ���� ������ ���ϰ� ��ȯ��
	FVector NewVector = FVector::ZeroVector;
	NewVector.Y = FMath::Sin(FMath::DegreesToRadians(Orientation)) * 1.0f;
	NewVector.X = FMath::Cos(FMath::DegreesToRadians(Orientation)) * 1.0f;
	NewVector.Normalize();
	return NewVector;
}

FVector AMyEnemyBear::SetTrace(FVector TargetVec) const
{
	// ���� Ʈ���̽� �߻� ��ġ��
	const FVector LeftStart = (GetActorLocation() + GetActorRightVector() + 80.0f) + GetActorForwardVector() * 50;
	const FVector LeftEnd = (GetActorLocation() + GetActorRightVector() + 80.0f) + GetActorForwardVector() * 400;

	// ���� Ʈ���̽� �߻� ��ġ��
	const FVector RightStart = (GetActorLocation() - GetActorRightVector() - 80.0f) + GetActorForwardVector() * 50;
	const FVector RightEnd = (GetActorLocation() - GetActorRightVector() - 80.0f) + GetActorForwardVector() * 400;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, LeftStart, LeftEnd, ECollisionChannel::ECC_WorldDynamic, FCollisionQueryParams(TEXT("ProjClient"), true, Instigator)) ||
		GetWorld()->LineTraceSingleByChannel(Hit, RightStart, RightEnd, ECollisionChannel::ECC_WorldDynamic, FCollisionQueryParams(TEXT("ProjClient"), true, Instigator))
		&& Hit.GetActor()->GetName() != "Landscape_10")
	{
		// Ʈ���̼� �浹 �� �浹 ��ġ�� ����� 200.0f �� ���� ���Ͽ� ��ȯ
		//TargetVec = Hit.ImpactPoint + Hit.Normal * 200.0f;
		return TargetVec;
	}
	else
	{
		return TargetVec;
	}
}

void AMyEnemyBear::AddDamage(const float Damage)
{
	// ü�� �谨 �� ü�¿� 0 ���Ͻ� ��� ���� ����
	CurrentHp -= Damage;
	if (CurrentHp <= 0.0f)
	{
		// ���� ���� �� �ݸ��� ���, ���� ����
		BearState = EBearState::Die;
		OutlineArea->SetSphereRadius(300.0f);
		BodyColl->SetCollisionResponseToAllChannels(ECR_Ignore);
		BodyColl->SetSimulatePhysics(false);
		// ���� �ð� �� ��ü �Ҹ�
		GetWorldTimerManager().SetTimer(DieTimer, this, &AMyEnemyBear::SetDestroy, DestoryTime, false);
	}
}

void AMyEnemyBear::SetDestroy()
{
	Destroy();
}

int AMyEnemyBear::GetRandomValue(const TArray<float>& Range)
{
	float Total = 0.0f;
	for (int32 i = 0; i < Range.Num(); ++i)
	{
		Total += Range[i];
	}
	float RandPoint = FMath::SRand() * Total;
	for (int32 i = 0; i < Range.Num(); ++i)
	{
		if (RandPoint < Range[i]) {
			return i;
		}
		else {
			RandPoint -= Range[i];
		}
	}
	return Range.Num() - 1;
}

void AMyEnemyBear::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		// ���� ���°� �ƴ� �� �߻�ü�� �浹�� ü�� ����
		if (OtherComp->GetName() == "ProjectileCollision" && BearState != EBearState::Die)
		{
			AAProjectile* Projectile = Cast<AAProjectile>(OtherActor);
			if (Projectile) {
				AddDamage(Projectile->Damage);

				// �ǰ� �ִϸ��̼��� ����� Ȯ��
				const TArray<float> RandValue = { 20, 80 };
				int Rand = GetRandomValue(RandValue);
				if (Rand == 0)
				{
					BearState = EBearState::Hit;
				}
			}
		}
	}
}

void AMyEnemyBear::OnOutlineOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OtherActor);
		if (Character) {
			if (BearState == EBearState::Die)
			{
				Character->IsDrop = true;
				SkeletalMesh->SetRenderCustomDepth(true);
			}
			else
			{
				BearState = EBearState::Arrive;
			}
		}
	}
}

void AMyEnemyBear::OnOurlineOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OtherActor);
		if (BearState == EBearState::Die && Character)
		{
			Character->IsDrop = false;
			SkeletalMesh->SetRenderCustomDepth(false);
		}
	}
}



