// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEchoOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar) HealthProgressBar->SetPercent(Percent);
}

void UEchoOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar) StaminaProgressBar->SetPercent(Percent);
}

void UEchoOverlay::SetGoldCount(int32 Gold)
{
	if (GoldCountText) GoldCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
}

void UEchoOverlay::SetSoulsCount(int32 Souls)
{
	if (SoulsCountText) SoulsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
}
