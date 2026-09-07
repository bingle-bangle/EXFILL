// Fill out your copyright notice in the Description page of Project Settings.

///////////////////////////
/*
	생성 날짜 : 2025.08.28

	생성자 : 백영기

	기능 : Item, Supply, Enemy 객체 스포너
*/
///////////////////////////

#pragma once

#include "SpawnerBase.h"
#include "EnemySpawner.generated.h"

/**
 *
 */
UCLASS()
class EXFIL_API AEnemySpawner : public ASpawnerBase
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	class AEnemy* mSpawnEnemy = nullptr;

	// 현재 스폰된 적 수
	int32		mSpawnedEnemyCount = 0;

	//랜덤 스폰 범위
	UPROPERTY(EditAnywhere)
	float		mSpawnRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	int32		mSpawnMaxCount = 1;

	bool		mIsSpawn = false;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemyForEscapeEvent(int32 spawncount, AController* player);
	void TestSpawn(int32 SpawnCount);
protected:
	//virtual void Spawn(int SpawnCount) override;
	virtual void Spawn_Implementation(int SpawnCount) override;


};
