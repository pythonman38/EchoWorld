// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"

class UInputComponent;
class UAttributeComponent;
class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class ECHOWORLD_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartEnemyPatrol();

	void CheckPatrolTarget();

	void DisplayEnemyHealthBar();

	void PlayHitReactMontage(const FName& SectionName);

	void Die();

	void PlayRandomDeathMontage();

	bool InTargetRange(AActor* Target, double Radius);

	void MoveToTarget(AActor* Target);

	void PatrolTimerFinished();

	AActor* ChoosePatrolTarget();

	void CheckCombatTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

private:
	FTimerHandle PatrolTimer;

	float WalkSpeed, RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	double CombatRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	double AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	double PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	float WaitTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	float WaitTimeMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EDeathPose DeathPose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Effects", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Attributes", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> PatrolTargets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AAIController> EnemyController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPawnSensingComponent> PawnSensing;
};
