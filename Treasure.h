// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Treasure.generated.h"

class USoundBase;

UCLASS()
class ECHOWORLD_API ATreasure : public AItem
{
	GENERATED_BODY()
	
public:
	ATreasure();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Treasure Properties", meta = (AllowPrivateAccess = "true"))
	int32 GoldAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> PickupSound;

	
};
