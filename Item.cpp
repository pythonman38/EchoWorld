// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "DrawDebugHelpers.h"
#include "DebugMacros.h"
#include "Components/SphereComponent.h"
#include "EchoCharacter.h"

// Sets default values
AItem::AItem() :
	Amplitude(0.25f),
	TimeConstant(5.f),
	ItemState(EItemState::EIS_Hovering)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComp"));
	ItemMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SetRootComponent(ItemMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::UpdateOverlappingItem(AActor* OtherActor, AItem* Item)
{
	TObjectPtr<AEchoCharacter> EchoCharacter = Cast<AEchoCharacter>(OtherActor);
	if (EchoCharacter) EchoCharacter->SetOverlappingItem(Item);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	UpdateOverlappingItem(OtherActor, this);
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherActorName = FString("Ending Overlap with: ") + OtherActor->GetName();
	UpdateOverlappingItem(OtherActor, nullptr);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering) AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}

