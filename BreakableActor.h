// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class ATreasure;
class UCapsuleComponent;

UCLASS()
class ECHOWORLD_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geo Collection", meta = (AllowPrivateAccess = "true"))
	bool bBroken;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geo Collection", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable Properties", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ATreasure>> TreasureClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Geo Collection", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> Capsule;

};
