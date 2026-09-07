// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EscapeEventSubsystem.generated.h"


class AExfilPlayerController;

UCLASS()
class EXFIL_API UEscapeEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()



public:
	UEscapeEventSubsystem();


public:

	static UEscapeEventSubsystem* GetEventSubsystem(const UObject* World);

	
	void StartRadioEvent(AController* Con);
	void StartFlareEvent(AController* con, FVector userlocation);

protected:





private:
	void RequestEscapeZoneEnable(FString escapename, FVector location);

	UFUNCTION()
	void EscapeUI(AExfilPlayerController* epc, FString itemname);

	FTimerHandle  mFlareActiveTimer;

	FTimerHandle mEscapeZoneActive;
	FTimerDelegate  mRadioDel;
	float	mRadioZoneTime=300.f;

	
};
