// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoCharacter.h"

#include "Animation/AnimMontage.h"
#include "AttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EchoHUD.h"
#include "EchoOverlay.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Item.h"
#include "Weapon.h"



// Sets default values
AEchoCharacter::AEchoCharacter() :
	CharacterState(ECharacterState::ECS_Unequipped),
	ActionState(EActionState::EAS_Unoccupied),
	bCarryingWeapon(false)
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

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

void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Jump);
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

void AEchoCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Unoccupied) Super::Jump();
}

float AEchoCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(Damage);
	if (EchoOverlay && Attributes) EchoOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	return Damage;
}

void AEchoCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f) ActionState = EActionState::EAS_HitReaction;
}

void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Player"));

	AddInputMappingContext();

	InitializeEchoOverlay();
}

void AEchoCharacter::InitializeEchoOverlay()
{
	auto PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		auto EchoHUD = Cast<AEchoHUD>(PlayerController->GetHUD());
		if (EchoHUD)
		{
			EchoOverlay = EchoHUD->GetEchoOverlay();
			if (EchoOverlay)
			{
				EchoOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				EchoOverlay->SetStaminaBarPercent(1.f);
				EchoOverlay->SetGoldCount(0);
				EchoOverlay->SetSoulsCount(0);
			}
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

void AEchoCharacter::FinishAction()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AEchoCharacter::Die()
{
	Super::Die();

	ActionState = EActionState::EAS_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEchoCharacter::AddInputMappingContext()
{
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
	if (OverlappingWeapon && !bCarryingWeapon) EquipWeapon(OverlappingWeapon);
	else
	{
		if (CanDisarm()) DisarmWeapon();
		else if (CanArm()) ArmWeapon();
	}
}

void AEchoCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->EquipWeapon(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_Equipped;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
	bCarryingWeapon = true;
}

void AEchoCharacter::ArmWeapon()
{
	PlayMontageSection(EquipMontage, FName("Equip"));
	CharacterState = ECharacterState::ECS_Equipped;
	ActionState = EActionState::EAS_Equipping;
}

void AEchoCharacter::DisarmWeapon()
{
	PlayMontageSection(EquipMontage, FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_Equipping;
}

bool AEchoCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && bCarryingWeapon;
}

bool AEchoCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped && bCarryingWeapon;
}

void AEchoCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttackMeshToSocket(GetMesh(), FName("SpineSocket"));
		CharacterState = ECharacterState::ECS_Unequipped;
	}
}

void AEchoCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttackMeshToSocket(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_Equipped;
	}
}