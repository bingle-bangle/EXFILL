// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "../Character/Enemy/Enemy.h"
#include "../Character/Enemy/EnemyAIController.h"
#include "../GameInstance/SpawnManager.h"


AEnemySpawner::AEnemySpawner()
{
	
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();


	USpawnManager::GetSpawnManager(GetWorld())->SpawnerSetting(FString(TEXT("Enemy")), this);
	//Spawn(mSpawnMaxCount);

}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemyForEscapeEvent(int32 spawncount, AController* player)
{
	if (!player) return;

	AActor* Player = player->GetPawn();

	if (!Player) return;

	for (int i = 0; i < mSpawnMaxCount; ++i)
	{
		if (IsValid(mSpawnClass))
		{

			if (IsValid(mSpawnParticle))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mSpawnParticle,
					GetActorLocation());
			}

			FActorSpawnParameters	param;
			// 겹치지 않게 알아서 액터 스폰 그게 힘든 상황이면 겹쳐서라도 강제스폰
			param.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FVector SpawnLocation = Player->GetActorLocation() + FVector(800.f, 0,0);

			// 스폰 포인트로부터 일정 범위 내 랜덤 스폰
			FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(150.f, mSpawnRadius);
			RandomOffset.Z = FVector::ZeroVector.Z;
			FVector OffsetSpawnLocation = SpawnLocation + RandomOffset;

			mSpawnEnemy = GetWorld()->SpawnActor<AEnemy>(mSpawnClass, OffsetSpawnLocation, GetActorRotation(), param);

			AController* AIController = mSpawnEnemy->GetController();
			if (AIController)
			{
				UBlackboardComponent* Board = Cast<AAIController>(AIController)->GetBlackboardComponent();
				if (Board)
				{
					// 이 스포너의 위치
					Board->SetValueAsVector(TEXT("SpawnLocation"), SpawnLocation);

					// 스폰범위만큼 탐색범위로 지정하기위해 블랙보드에 값넘겨줌
					Board->SetValueAsFloat(TEXT("SpawnRadius"), mSpawnRadius);

					Board->SetValueAsObject(TEXT("Target"), Player);
					
				}
			}
		}
	}
}

void AEnemySpawner::TestSpawn(int32 SpawnCount)
{
	for (int i = 0; i < mSpawnMaxCount; ++i)
	{
		if (IsValid(mSpawnClass))
		{

			if (IsValid(mSpawnParticle))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mSpawnParticle,
					GetActorLocation());
			}

			FActorSpawnParameters	param;
			// 겹치지 않게 알아서 액터 스폰 그게 힘든 상황이면 겹쳐서라도 강제스폰
			param.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FVector SpawnLocation = GetActorLocation();

			// 스폰 포인트로부터 일정 범위 내 랜덤 스폰
			FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(100.f, mSpawnRadius);
			RandomOffset.Z = FVector::ZeroVector.Z;
			FVector OffsetSpawnLocation = SpawnLocation + RandomOffset;

			mSpawnEnemy = GetWorld()->SpawnActor<AEnemy>(mSpawnClass, OffsetSpawnLocation, GetActorRotation(), param);

			AController* AIController = mSpawnEnemy->GetController();
			if (AIController)
			{
				UBlackboardComponent* Board = Cast<AAIController>(AIController)->GetBlackboardComponent();
				if (Board)
				{
					// 이 스포너의 위치
					Board->SetValueAsVector(TEXT("SpawnLocation"), GetActorLocation());

					// 스폰범위만큼 탐색범위로 지정하기위해 블랙보드에 값넘겨줌
					Board->SetValueAsFloat(TEXT("SpawnRadius"), mSpawnRadius);
				}
			}
		}
	}

}

void AEnemySpawner::Spawn_Implementation(int SpawnCount)
{
	for (int i = 0; i < mSpawnMaxCount; ++i)
	{
		if (IsValid(mSpawnClass))
		{

			if (IsValid(mSpawnParticle))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mSpawnParticle,
					GetActorLocation());
			}

			FActorSpawnParameters	param;
			// 겹치지 않게 알아서 액터 스폰 그게 힘든 상황이면 겹쳐서라도 강제스폰
			param.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FVector SpawnLocation = GetActorLocation();

			// 스폰 포인트로부터 일정 범위 내 랜덤 스폰
			FVector RandomOffset = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(100.f, mSpawnRadius);
			RandomOffset.Z = FVector::ZeroVector.Z;
			FVector OffsetSpawnLocation = SpawnLocation + RandomOffset;

			mSpawnEnemy = GetWorld()->SpawnActor<AEnemy>(mSpawnClass, OffsetSpawnLocation, GetActorRotation(), param);

			AController* AIController = mSpawnEnemy->GetController();
			if (AIController)
			{
				UBlackboardComponent* Board = Cast<AAIController>(AIController)->GetBlackboardComponent();
				if (Board)
				{
					// 이 스포너의 위치
					Board->SetValueAsVector(TEXT("SpawnLocation"), GetActorLocation());

					// 스폰범위만큼 탐색범위로 지정하기위해 블랙보드에 값넘겨줌
					Board->SetValueAsFloat(TEXT("SpawnRadius"), mSpawnRadius);
				}
			}
		}
	}
}