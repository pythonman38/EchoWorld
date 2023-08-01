// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "EchoCharacter.h"
#include "HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon() :
	Damage(20.f),
	bShowBoxDebug(false),
	BoxTraceExtent(FVector(5.f))
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
}

void AWeapon::EquipWeapon(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator, bool bIsCharacter)
{
	SetItemState(EItemState::EIS_Equipped);
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttackMeshToSocket(InParent, InSocketName);
	if (GetSphere()) GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (EquipSound && bIsCharacter) UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	if (GetEmbersEffect()) GetEmbersEffect()->Deactivate();
}

void AWeapon::AttackMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	GetItemMesh()->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"))) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		if (GetOwner()->ActorHasTag(TEXT("Enemy")) && BoxHit.GetActor()->ActorHasTag(TEXT("Enemy"))) return;
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		ExecuteGetHit(BoxHit);
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation(), End = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors) ActorsToIgnore.AddUnique(Actor);

	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, BoxTraceExtent, BoxTraceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, 
		ActorsToIgnore, bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, BoxHit, true);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface) HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
}
