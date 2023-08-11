// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOWORLD_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float Damage);

	void UseStamina(float StaminaCost);

	float GetHealthPercent();

	float GetStaminaPercent();

	void RegenerateStamina(float DeltaTime);

	bool IsAlive();

	void AddSouls(int32 NumberOfSouls);

	void AddGold(int32 AmountOfGold);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	int32 Souls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float DodgeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = "true"))
	float StaminaRegenRate;
	
public:
	// Getters for private variables
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE int32 GetGoldAmount() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
};
