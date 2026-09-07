// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.generated.h"

UCLASS()
class EXFIL_API ARangedWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARangedWeapon();
	virtual void Tick(float DeltaTime) override;
protected:
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent>	mMesh;

	
	
};
