// Fill out your copyright notice in the Description page of Project Settings.

///////////////////////////
/*
	생성 날짜 : 2025.08.28

	생성자 : 백영기

	기능 :
*/
///////////////////////////

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "SpawnerBase.generated.h"

UCLASS()
class EXFIL_API ASpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerBase();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent>	mRoot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor>			mSpawnClass;

	UPROPERTY(EditAnywhere)
	float						mSpawnTime;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem>	mSpawnParticle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(Server, Reliable)
	virtual void Spawn(int SpawnCount);

	virtual void Spawn_Implementation(int SpawnCount);

};
