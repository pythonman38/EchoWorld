// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "EchoCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class ECHOWORLD_API AEchoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEchoCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called for movement input
	void Move(const FInputActionValue& Value);

	// Called for looking input
	void Look(const FInputActionValue& Value);

	// Called to allow the Character to pickup a Weapon
	void EKeyPressed();

	// Called to allow Character to attack Enemies
	void Attack();

	// Called from Montage to set ActionState to Unoccupied
	UFUNCTION(BlueprintCallable)
	void FinishAttacking();

	// Plays attack Montage when Action State is Unoccupied
	void PlayAttackMontage();

	void PlayEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void DisarmWeapon();

	UFUNCTION(BlueprintCallable)
	void ArmWeapon();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	bool CarryingWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hair, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hair, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AItem> OverlappingItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWeapon> EquippedWeapon;

public:
	// Getters for private variables
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	// Setters for private variables
	FORCEINLINE void SetOverlappingItem(TObjectPtr<AItem> Item) { OverlappingItem = Item; }

};
