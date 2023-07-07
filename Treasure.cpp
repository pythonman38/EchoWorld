// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "EchoCharacter.h"
#include "Kismet/GameplayStatics.h"

ATreasure::ATreasure()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.f, 45.f * DeltaTime, 0.f));
}

void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(OtherActor);
	if (EchoCharacter)
	{
		if (PickupSound) UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		Destroy();
	}
}
