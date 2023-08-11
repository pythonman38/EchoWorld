// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul.h"

#include "PickupInterface.h"

ASoul::ASoul() :
	Souls(10)
{
}

void ASoul::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddSouls(this);

		SpawnPickupSystem();
		SpawnPickupSound();

		Destroy();
	}
}
