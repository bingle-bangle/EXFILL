// Fill out your copyright notice in the Description page of Project Settings.

///////////////////////////
/*
	생성 날짜 : 2025.09.11

	생성자 : 백영기

	기능 : Item, Supply, Enemy 객체 스포너
*/
///////////////////////////

#pragma once

#include "SpawnerBase.h"
#include "TurretSpawner.generated.h"

/**
 *
 */
UCLASS()
class EXFIL_API ATurretSpawner : public ASpawnerBase
{
	GENERATED_BODY()


	ATurretSpawner();


protected:
	class ATurret* mSpawnTurret = nullptr;

	bool		mIsSpawn = false;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SpawnTurret(int32 SpawnCount);
protected:
	virtual void Spawn_Implementation(int SpawnCount);
};
