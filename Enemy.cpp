// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"
#include "HealthBarComponent.h"

// Sets default values
AEnemy::AEnemy() :
	DeathPose(EDeathPose::EDP_Alive),
	CombatRadius(500.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(1.f);
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Die()
{
	PickRandomDeathMontage();

	if (HealthBarWidget) HealthBarWidget->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

void AEnemy::PickRandomDeathMontage()
{
	auto AnimInstance = GetMesh()->GetAnimInstance();
	const int32 Selection = FMath::RandRange(0, 4);
	FName SectionName = FName();
	switch (Selection)
	{
	case 0:
		SectionName = FName("Death1");
		DeathPose = EDeathPose::EDP_Death1;
		break;
	case 1:
		SectionName = FName("Death2");
		DeathPose = EDeathPose::EDP_Death2;
		break;
	case 2:
		SectionName = FName("Death3");
		DeathPose = EDeathPose::EDP_Death3;
		break;
	case 3:
		SectionName = FName("Death4");
		DeathPose = EDeathPose::EDP_Death4;
		break;
	case 4:
		SectionName = FName("Death5");
		DeathPose = EDeathPose::EDP_Death5;
		break;
	}

	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget) HealthBarWidget->SetVisibility(true);

	if (Attributes && Attributes->IsAlive()) DirectionalHitReact(ImpactPoint);
	else Die();

	if (HitSound) UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);

	if (HitParticles) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector(), ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z), ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0) Theta *= -1.f;

	FName Section("ReactBack");
	if (Theta >= -45.f && Theta < 45.f) Section = FName("ReactFront");
	else if (Theta >= -135.f && Theta < -45.f) Section = FName("ReactLeft");
	else if (Theta >= 45.f && Theta < 135.f) Section = FName("ReactRight");

	PlayHitReactMontage(Section);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
		if (DistanceToTarget > CombatRadius)
		{
			CombatTarget = nullptr;
			if (HealthBarWidget) HealthBarWidget->SetVisibility(false);
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(Damage);
		if (HealthBarWidget) HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}

	CombatTarget = EventInstigator->GetPawn();

	return Damage;
}