// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Soul.generated.h"

UCLASS()
class ECHOWORLD_API ASoul : public AItem
{
	GENERATED_BODY()

public:
	ASoul();
	
protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul Properties", meta = (AllowPrivateAccess = "true"))
	int32 Souls;

public:
	// Getters for private variables
	FORCEINLINE int32 GetSouls() const { return Souls; }

	// Setters for private variables
	FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }
};
