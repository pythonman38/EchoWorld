// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoHUD.h"

#include "EchoOverlay.h"

void AEchoHUD::BeginPlay()
{
	Super::BeginPlay();

	auto World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && EchoOverlayClass)
		{
			EchoOverlay = CreateWidget<UEchoOverlay>(Controller, EchoOverlayClass);
			EchoOverlay->AddToViewport();
		}
	}
}
