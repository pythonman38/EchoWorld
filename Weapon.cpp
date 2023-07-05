// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "EchoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

void AWeapon::EquipWeapon(USceneComponent* InParent, FName InSocketName)
{
	AttackMeshToSocket(InParent, InSocketName);
	SetItemState(EItemState::EIS_Equipped);
	if (EquipSound) UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	if (GetSphere()) GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::AttackMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	GetItemMesh()->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
