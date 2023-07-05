// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped")
};

UCLASS()
class ECHOWORLD_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	void UpdateOverlappingItem(AActor* OtherActor, AItem* Item);

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Sine Parameters", meta = (AllowPrivateAccess))
	float RunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sine Parameters", meta = (AllowPrivateAccess))
	float Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sine Parameters", meta = (AllowPrivateAccess))
	float TimeConstant;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess))
	EItemState ItemState;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Static Mesh", meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Static Mesh", meta = (AllowPrivateAccess))
	TObjectPtr<USphereComponent> Sphere;

public:
	// Getters for private variables
	FORCEINLINE EItemState GetItemState() const { return ItemState; }
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetItemMesh() const { return ItemMesh; }
	FORCEINLINE TObjectPtr<USphereComponent> GetSphere() const { return Sphere; }

	// Setters for private variables
	FORCEINLINE void SetItemState(EItemState State) { ItemState = State; }
};
