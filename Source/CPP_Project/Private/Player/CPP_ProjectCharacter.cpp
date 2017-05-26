// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CPP_Project.h"
#include "AProjectile.h"
#include "BaseItem.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "CPP_ProjectCharacter.h"

ACPP_ProjectCharacter::ACPP_ProjectCharacter()
{

	CurrentState = EPlayerState::PS_Idle;

	/* 카메라 변수 */
	MaxHP = 100.0f;
	CurrentHP = MaxHP;
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	/* 무브먼트 변수 */
	MovmentRunSpeed = 800.0f;
	MovementWalkSpeed = 500.0f;
	CrouchSpeed = 100.0f;

	/* 애니메이션 변수 */
	AnimCrouch = false;
	AnimReload = false;
	AnimFire = false;
	bRunning = false;
	IsFire = true;
	IsDrop = false;

	// 캡슐 컴포넌트 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// 카메라의 회전 설정
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 150.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = MovementWalkSpeed;

	// 카메라 암 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; 
	CameraBoom->bUsePawnControlRotation = false;

	// 카메라 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	// 캐릭터 스켈레탈 메쉬 설정
	//Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	//Mesh = GetMesh();

	// 웨펀 스켈레탈 메쉬 설정
	static ConstructorHelpers::FObjectFinder <USkeletalMesh> IWeaponMesh(TEXT("SkeletalMesh'/Game/Model/Rifle/Rifle.Rifle'"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterWeaponMesh"));
	WeaponMesh->AttachTo(GetMesh(), TEXT("Weapon_Point"));
	WeaponMesh->SetSkeletalMesh(IWeaponMesh.Object);
	MuzzleSocketName = TEXT("MuzzleFlashSocket");

}

void ACPP_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	ItemManager = NewObject<UUItemManager>(this);
	BasicWeapon = NewObject<UWeapon>(this, UWeapon::StaticClass());
	if (BasicWeapon)
	{
		BasicWeapon->Initialize(this, GetWorld()->GetFirstPlayerController()->GetHUD());
	}
}

void ACPP_ProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	BasicWeapon->UpdateWeapon(DeltaSeconds);
}

void ACPP_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPP_ProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPP_ProjectCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACPP_ProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACPP_ProjectCharacter::LookUpAtRate);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &ACPP_ProjectCharacter::OnCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &ACPP_ProjectCharacter::OffCrouch);
	InputComponent->BindAction("Run", IE_Pressed, this, &ACPP_ProjectCharacter::StartRun);
	InputComponent->BindAction("Run", IE_Released, this, &ACPP_ProjectCharacter::StopRun);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ACPP_ProjectCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ACPP_ProjectCharacter::StopFire);
	InputComponent->BindAction("Reload", IE_Pressed, this, &ACPP_ProjectCharacter::OnReload);
}

void ACPP_ProjectCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACPP_ProjectCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACPP_ProjectCharacter::UpdateBuild()
{
	if (BuildItem != NULL) {
		FHitResult Hit;
		const FVector Start = GetMesh()->GetSocketLocation(TEXT("Head_Point"));
		const FVector End = (GetActorForwardVector() * 1000.0f) + Start;

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams(TEXT("Build Trace"), true, Instigator)))
		{
			BuildItem->SetBuildItem(Hit.ImpactPoint, true);
		} else {
			BuildItem->SetBuildItem(End, false);
		}
	}
}

void ACPP_ProjectCharacter::OnBuild()
{
	// 아이템 설치 후 변수들 초기화
	if (BuildItem->GetBuildCheck()) {
		IsFire = true;
		GetWorldTimerManager().ClearTimer(BuildTimer);
		BuildItem->OnBuild();
		BuildItem = NULL;
	}
}

void ACPP_ProjectCharacter::GetItemClass(TSubclassOf<ABaseItem> GetItemClass)
{
	// 아이템을 레벨에 스폰 및 타이머를 사용하여 Update 함.
	if (GetItemClass)
	{
		IsFire = false;
		BuildItem = GetWorld()->SpawnActor<ABaseItem>(GetItemClass, GetActorLocation() , FRotator::ZeroRotator);
		BuildItem->SetOriginalMaterial();
		GetWorldTimerManager().SetTimer(BuildTimer, this, &ACPP_ProjectCharacter::UpdateBuild, 0.01f, true);
	}
}

void ACPP_ProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACPP_ProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && (!bRunning))
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACPP_ProjectCharacter::StartFire()
{
	if (IsFire) {
		BasicWeapon->StartFire();
		AnimFire = true;
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	} else {
		OnBuild();
	}
}

void ACPP_ProjectCharacter::StopFire()
{
	BasicWeapon->StopFire();
	AnimFire = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementWalkSpeed;
}


void ACPP_ProjectCharacter::OnReload()
{
	AnimReload = true;
	BasicWeapon->SetWeaponState(EMyWeaponState::Reload);
}

void ACPP_ProjectCharacter::OnCrouch()
{
	AnimCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
}

void ACPP_ProjectCharacter::OffCrouch()
{
	AnimCrouch = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementWalkSpeed;
}

void ACPP_ProjectCharacter::StartRun()
{
	if (!AnimFire) {
		bRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = MovmentRunSpeed;
	}
}

void ACPP_ProjectCharacter::StopRun()
{
	bRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementWalkSpeed;
}

void ACPP_ProjectCharacter::StartJump()
{
	bPressedJump = true;
}

void ACPP_ProjectCharacter::StopJump()
{
	bPressedJump = false;
}


