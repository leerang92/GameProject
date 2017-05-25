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
	IsAttack = false;

	// 추격 변수들
	MoveSpeed = 300.0f;
	LookSpeed = 200.0f;
	Angular = 0.0f;
	SetRot = 0.0f;
	SetOrientation = 0.0f;
	UpdateRotation = 0.0f;
	DestoryTime = 15.0f;
	Velocity = FVector::ZeroVector;

	// 배회 변수들
	Offset = 60.0f;
	Radius = 3.0f;
	Rate = 60.0f;

	// 상태 거리 범위들
	NearDistance = 1400.0f;
	FarDistance = 1500.0f;
	AttackDistance = 200.0f;

	// 캡슐 컬리더 설정
	BodyColl = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	BodyColl->SetCapsuleHalfHeight(108.497429f);
	BodyColl->SetCapsuleRadius(108.497429f);
	BodyColl->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemyBear::OnOverlapBegin);
	BodyColl->SetEnableGravity(true);
	//CapsuleColl->SetSimulatePhysics(true);
	RootComponent = BodyColl;

	/* 드랍 가능 및 아웃라인 표시할 스피어 콜리더 설정 */
	OutlineAreaColl = CreateDefaultSubobject<USphereComponent>(TEXT("Shpere Collision"));
	OutlineAreaColl->SetSphereRadius(100.0f);
	OutlineAreaColl->AttachTo(RootComponent);
	OutlineAreaColl->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemyBear::OnOverlapBegin);

	/* 공격 가능 범위 체크할 캡슐 콜리더 설정 */
	AttackColl = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Attack Area"));
	/*AttackColl->SetCapsuleHalfHeight(130.0f);
	AttackColl->SetCapsuleRadius(80.0f);
	AttackColl->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(90.0f, 0, 0));
	AttackColl->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemyBear::OnOverlapAttack);
	AttackColl->OnComponentEndOverlap.AddDynamic(this, &AMyEnemyBear::OnOverlapAttackEnd);*/
	AttackColl->AttachTo(BodyColl);


	// 스켈레탈 메쉬 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(
		TEXT("SkeletalMesh'/Game/Model/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear.Enemy_Bear'"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bear Skeletal Mesh Componenet"));
	SkeletalMesh->SetRelativeLocationAndRotation(FVector(0, 0, -120.0f), FRotator(0, -90.0f, 0));
	SkeletalMesh->SetSkeletalMesh(SK_Mesh.Object);
	SkeletalMesh->SetupAttachment(BodyColl);

	// 애니메이션 블루프린트 설정
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> BP_Anim(
		TEXT("AnimBlueprint'/Game/Model/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear_Animations/EnemyBearAnimBP.EnemyBearAnimBP'"));
	SkeletalMesh->SetAnimInstanceClass(BP_Anim.Object->GeneratedClass);

	// 무브먼트 설정
	BearMoveComp = CreateDefaultSubobject<UBearMovementComponent>(TEXT("BearMovementComponent"));
	BearMoveComp->UpdatedComponent = RootComponent;
}

void AMyEnemyBear::BeginPlay()
{
	Super::BeginPlay();
}

void AMyEnemyBear::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// 플레이어와의 거리 구하기
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GWorld, 0);
	float Distance = GetDistanceTo(Character);

	// 체력이 0 이상일 시 상태 변경
	if (CurrentHp > 0.0f) {
		// 공격 위치에 들어왔을 시
		if (Distance <= AttackDistance && IsAttack)
		{
			BearState = EMyBearState::Attack;
			SetAttack();
		}
		// 멀리 떨어졌을 시
		else if (Distance > NearDistance) {
			BearState = EMyBearState::Move;
			StateWander(DeltaTime);
		}
		// 추격 위치에 들어왔을 시
		else if (Distance < NearDistance)
		{
			BearState = EMyBearState::Move;
			SetArrive(DeltaTime);
		}
	}
}

UPawnMovementComponent * AMyEnemyBear::GetMovementComponent() const
{
	return BearMoveComp;
}

EMyBearState::Type AMyEnemyBear::GetBearState() const
{
	return BearState;
}

// 플레이어를 추격하여 이동
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
	// 캐릭터의 위치를 구함.
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GWorld, 0);
	FVector TraceVec = SetTrace(Character->GetActorLocation());
	LookAtTarget(TraceVec, DeltaTime);

	// 정면으로 이동
	BearMoveComp->AddInputVector(GetActorForwardVector() * MoveSpeed * DeltaTime);
}

void AMyEnemyBear::SetAttack()
{
	UE_LOG(LogClass, Warning, TEXT("Attack!!"));
}

void AMyEnemyBear::LookAtTarget(FVector TargetLocation, float DeltaTime)
{
	// 캐릭터와의 거리를 구함
	// Atan2()를 사용하여 Y, X 두방향의 각도를 구함 
	FVector Direction = TargetLocation - GetActorLocation();
	float Orientation = FMath::Atan2(Direction.Y, Direction.X);
	Orientation = FMath::RadiansToDegrees(Orientation);

	// 실제 각도를 구함
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
	// 타겟을 향해 회전
	LookAtTarget(TargetPosition, DeltaTime);

	// 타겟을 향해 이동
	FVector Direction = TargetPosition - GetActorLocation();
	Direction.Normalize();
	BearMoveComp->AddInputVector(Direction * MoveSpeed * DeltaTime);
}

FVector AMyEnemyBear::GetDirection(float Orientation)
{
	// 타겟의 방향을 구하고 반환함
	FVector NewVector = FVector::ZeroVector;
	NewVector.Y = FMath::Sin(FMath::DegreesToRadians(Orientation)) * 1.0f;
	NewVector.X = FMath::Cos(FMath::DegreesToRadians(Orientation)) * 1.0f;
	NewVector.Normalize();
	return NewVector;
}

FVector AMyEnemyBear::SetTrace(FVector TargetVec) const
{
	// 좌측 트레이스 발사 위치들
	const FVector LeftStart = (GetActorLocation() + GetActorRightVector() + 80.0f) + GetActorForwardVector()  * 50;
	const FVector LeftEnd = (GetActorLocation() + GetActorRightVector() + 80.0f) + GetActorForwardVector() * 400;

	// 우측 트레이스 발사 위치들
	const FVector RightStart = (GetActorLocation() - GetActorRightVector() - 80.0f) + GetActorForwardVector() * 50;
	const FVector RightEnd = (GetActorLocation() - GetActorRightVector() - 80.0f) + GetActorForwardVector() * 400;
	
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, LeftStart, LeftEnd, ECollisionChannel::ECC_WorldDynamic, FCollisionQueryParams(TEXT("ProjClient"), true, Instigator)) || 
		GetWorld()->LineTraceSingleByChannel(Hit, RightStart, RightEnd, ECollisionChannel::ECC_WorldDynamic, FCollisionQueryParams(TEXT("ProjClient"), true, Instigator)))
	{
		// 트레이서 충돌 시 충돌 위치에 방향과 200.0f 의 값을 더하여 반환
		TargetVec = Hit.ImpactPoint + Hit.Normal * 200.0f;
		return TargetVec;
	}	
	else
	{
		return TargetVec;
	}
}

void AMyEnemyBear::AddDamage(float Damage)
{
	// 체력 삭감 및 체력에 0 이하시 사망 상태 변경
	CurrentHp -= Damage;
	if (CurrentHp <= 0.0f)
	{
		// 상태 변경 및 콜리더 통과, 물리 제거
		BearState = EMyBearState::Die;
		BodyColl->SetCollisionResponseToAllChannels(ECR_Ignore);
		BodyColl->SetSimulatePhysics(false);
		// 일정 시간 후 객체 소멸
		GetWorldTimerManager().SetTimer(DieTimer, this, &AMyEnemyBear::SetDestroy, DestoryTime, false);
	}
}

void AMyEnemyBear::SetDestroy()
{
	Destroy();
}

void AMyEnemyBear::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) 
	{
		// 죽음 상태가 아닐 때 발사체와 충돌시 체력 감소
		if (OtherComp->GetName() == "ProjectileCollision" && BearState != EMyBearState::Die)
		{
			AAProjectile* Projectile = Cast<AAProjectile>(OtherActor);
			AddDamage(Projectile->Damage);
		}
	}
}

void AMyEnemyBear::OnOverlapAttack(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		// 플레이어가 공격 범위에 들어왔을 시 공격 실행
		ACPP_ProjectCharacter* Character = Cast<ACPP_ProjectCharacter>(OtherActor);
		if (Character)
		{
			IsAttack = true;
		}
		
	}
}

void AMyEnemyBear::OnOverlapAttackEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		UE_LOG(LogClass, Warning, TEXT("End Player Character !! "));
	}
}



