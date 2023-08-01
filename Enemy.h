// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"

class UInputComponent;
class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;
class AWeapon;

UCLASS()
class ECHOWORLD_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

protected:
	/** <ABaseCharacter> */
	virtual void Die() override;

	virtual void Attack() override;

	virtual void HandleDamage(float Damage) override;

	virtual int32 PlayDeathMontage() override;

	virtual void FinishAction() override;
	/** </ABaseCharacter> */

protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	void SpawnDefaultWeapon();

	void StartEnemyPatrol();

	void CheckPatrolTarget();

	void DisplayEnemyHealthBar();

	bool InTargetRange(AActor* Target, double Radius);

	void MoveToTarget(AActor* Target);

	void PatrolTimerFinished();

	void PatrolAndOrAttack();

	AActor* ChoosePatrolTarget();

	void CheckCombatTarget();

	void StartPatrolling();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	void ChaseTarget();

	void StartAttackTimer();

	bool CanAttack();

private:
	FTimerHandle PatrolTimer;

	float PatrolSpeed, ChaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	double CombatRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	double AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	double PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	float PatrolWaitMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	float PatrolWaitMax;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> PatrolTargets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AAIController> EnemyController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPawnSensingComponent> PawnSensing;
};
