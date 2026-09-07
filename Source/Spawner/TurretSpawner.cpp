// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretSpawner.h"
#include "../Character/Enemy/Turret.h"
#include "../GameInstance/SpawnManager.h"

ATurretSpawner::ATurretSpawner()
{

}

void ATurretSpawner::BeginPlay()
{
	Super::BeginPlay();

	//USpawnManager::GetSpawnManager(GetWorld())->SpawnerSetting(FString(TEXT("Turret")), this);
	//Spawn(1);

}

void ATurretSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurretSpawner::SpawnTurret(int32 SpawnCount)
{
	FActorSpawnParameters	param;

	FVector Location = GetActorLocation();

	mSpawnTurret = GetWorld()->SpawnActor<ATurret>(mSpawnClass, Location, GetActorRotation(), param);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Spawn Turret"));
}

void ATurretSpawner::Spawn_Implementation(int SpawnCount)
{
	FActorSpawnParameters	param;

	FVector Location = GetActorLocation();

	mSpawnTurret = GetWorld()->SpawnActor<ATurret>(mSpawnClass, Location, GetActorRotation(), param);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Spawn Turret"));
}