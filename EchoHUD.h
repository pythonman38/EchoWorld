// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EchoHUD.generated.h"

class UEchoOverlay;

UCLASS()
class ECHOWORLD_API AEchoHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Echo, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UEchoOverlay> EchoOverlayClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Echo, meta  = (AllowPrivateAccess = "true"))
	TObjectPtr<UEchoOverlay> EchoOverlay;

public:
	// Getters for private variables
	FORCEINLINE UEchoOverlay* GetEchoOverlay() const { return EchoOverlay; }
};
