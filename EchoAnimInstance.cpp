// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoAnimInstance.h"

#include "EchoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEchoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EchoCharacter = Cast<AEchoCharacter>(TryGetPawnOwner());
}

void UEchoAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (EchoCharacter == nullptr) EchoCharacter = Cast<AEchoCharacter>(TryGetPawnOwner());

	if (EchoCharacter)
	{
		// Get the lateral speed of the character from velocity
		Speed = UKismetMathLibrary::VSizeXY(EchoCharacter->GetCharacterMovement()->Velocity);

		// Is the character in the air?
		bIsInAir = EchoCharacter->GetCharacterMovement()->IsFalling();

		// Is the character moving?
		bIsMoving = EchoCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0;

		// What is the current state of the Character?
		CharacterState = EchoCharacter->GetCharacterState();

		// What is the current action of the Character?
		ActionState = EchoCharacter->GetActionState();

		// How did the Character die?
		DeathPose = EchoCharacter->GetDeathPose();
	}
}
