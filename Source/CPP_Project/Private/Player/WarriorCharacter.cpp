// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Project.h"
#include "WarriorCharacter.h"


// Sets default values
AWarriorCharacter::AWarriorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHP = 100.0f;
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	MovementRunSpeed = 800.0f;
	MovementWalkSpeed = 500.0f;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540.0f, 0);
	GetCharacterMovement()->JumpZVelocity = 300.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = MovementWalkSpeed;

	// 카메라 암 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// 카메라 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 웨펀 스켈레탈 메쉬 설정
	static ConstructorHelpers::FObjectFinder <USkeletalMesh> IWeaponMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_Balloon/SK_Blade_Balloon.SK_Blade_Balloon'"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterWeaponMesh"));
	WeaponMesh->AttachTo(GetMesh(), TEXT("WeaponSocket"));
	WeaponMesh->SetSkeletalMesh(IWeaponMesh.Object);
}

// Called when the game starts or when spawned
void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
}

// Called every frame
void AWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWarriorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWarriorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWarriorCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWarriorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWarriorCharacter::LookUpAtRate);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AWarriorCharacter::StartAttack);
	InputComponent->BindAction("Fire", IE_Released, this, &AWarriorCharacter::StopAttack);
}

EPlayerState AWarriorCharacter::GetPlayerState() const
{
	return CurrentState;
}

void AWarriorCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (CurrentState != EPlayerState::PS_Attack))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AWarriorCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AWarriorCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWarriorCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AWarriorCharacter::StartAttack()
{
	if(CurrentState != EPlayerState::PS_Attack)
		CurrentState = EPlayerState::PS_Attack;
}

void AWarriorCharacter::StopAttack()
{
	if(CurrentState == EPlayerState::PS_Attack)
		CurrentState = EPlayerState::PS_Idle;
}

void AWarriorCharacter::UpdateBuild()
{
	if (BuildItem != NULL) {
		FHitResult Hit;
		const FVector Start = GetMesh()->GetSocketLocation(TEXT("Head_Point"));
		const FVector End = (GetActorForwardVector() * 1000.0f) + Start;

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams(TEXT("Build Trace"), true, Instigator)))
		{
			BuildItem->SetBuildItem(Hit.ImpactPoint, true);
		}
		else {
			BuildItem->SetBuildItem(End, false);
		}
	}
}

void AWarriorCharacter::GetItemClass(TSubclassOf<ABaseItem> GetItemClass)
{
	// 아이템을 레벨에 스폰 및 타이머를 사용하여 Update 함.
	if (GetItemClass)
	{
		BuildItem = GetWorld()->SpawnActor<ABaseItem>(GetItemClass, GetActorLocation(), FRotator::ZeroRotator);
		BuildItem->SetOriginalMaterial();
		GetWorldTimerManager().SetTimer(BuildTimer, this, &AWarriorCharacter::UpdateBuild, 0.01f, true);
	}
}

void AWarriorCharacter::OnBuild()
{
	// 아이템 설치 후 변수들 초기화
	if (BuildItem->GetBuildCheck()) {
		GetWorldTimerManager().ClearTimer(BuildTimer);
		BuildItem->OnBuild();
		BuildItem = NULL;
	}
}

void AWarriorCharacter::GetDamage(float Damage)
{
	CurrentHP -= Damage;

	if (CurrentHP <= 0.0f)
	{
		CurrentState = EPlayerState::PS_Die;
	}
}

