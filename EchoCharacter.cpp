// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Item.h"
#include "Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

// Sets default values
AEchoCharacter::AEchoCharacter() :
	CharacterState(ECharacterState::ECS_Unequipped),
	ActionState(EActionState::EAS_Unoccupied),
	CarryingWeapon(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a spring arm and attach it to the camera;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a camera and attach it to the root component
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

// Called when the game starts or when spawned
void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AEchoCharacter::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller != nullptr)
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward and right vectors
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AEchoCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(-LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AEchoCharacter::EKeyPressed()
{
	TObjectPtr<AWeapon> OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon && !CarryingWeapon)
	{
		OverlappingWeapon->EquipWeapon(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_Equipped;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
		CarryingWeapon = true;
	}
	else
	{
		if (ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped && CarryingWeapon)
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_Equipping;
		}
		else if (ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && CarryingWeapon)
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_Equipped;
			ActionState = EActionState::EAS_Equipping;
		}
	}
}

void AEchoCharacter::Attack()
{
	if (CharacterState == ECharacterState::ECS_Unequipped) return;

	if (ActionState == EActionState::EAS_Unoccupied)
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void AEchoCharacter::FinishAttacking()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AEchoCharacter::PlayAttackMontage()
{
	auto AnimInstance = GetMesh()->GetAnimInstance();
	const int32 Selection = FMath::RandRange(0, 1);
	FName SectionName = FName();
	switch (Selection)
	{
	case 0:
		SectionName = FName("Attack1");
		break;
	case 1:
		SectionName = FName("Attack2");
		break;
	}

	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEchoCharacter::PlayEquipMontage(const FName& SectionName)
{
	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AEchoCharacter::DisarmWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttackMeshToSocket(GetMesh(), FName("SpineSocket"));
		CharacterState = ECharacterState::ECS_Unequipped;
	}
}

void AEchoCharacter::ArmWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttackMeshToSocket(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_Equipped;
	}
}

void AEchoCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

// Called every frame
void AEchoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Look);

		// Equipping
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AEchoCharacter::EKeyPressed);

		// Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Attack);
	}
}

