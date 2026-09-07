// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "EscapeZone.h"
#include "RadioEscapeZone.generated.h"

/**
 * 
 */
UCLASS()
class EXFIL_API ARadioEscapeZone : public AEscapeZone
{
	GENERATED_BODY()
	
	
public:
	ARadioEscapeZone();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	

	virtual void OnPlayerEnter(APlayerState* state);
	virtual void OnPlayerOut(APlayerState* state);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent>	mMesh;

	float mAlivetime = 0;

};
