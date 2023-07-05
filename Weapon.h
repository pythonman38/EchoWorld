// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class USoundBase;

UCLASS()
class ECHOWORLD_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	void EquipWeapon(USceneComponent* InParent, FName InSocketName);

	void AttackMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> EquipSound;
};
