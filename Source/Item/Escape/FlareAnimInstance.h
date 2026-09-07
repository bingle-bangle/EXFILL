// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Animation/AnimInstance.h"
#include "FlareAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXFIL_API UFlareAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetMainRotorSpeed(float mainrotorspeed);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetTailRotorSpeed(float tailrotorspeed);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateRearDoors(float left,float right);


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator	mMainRotorSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator	mTailRotorSpeed;
};
