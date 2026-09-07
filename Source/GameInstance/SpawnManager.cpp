// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "ItemDataSubsystem.h"
#include "../Spawner/Item/ItemSpawner.h"
#include "../Spawner/EnemySpawner.h"
#include "../Item/SupplyItem.h"
#include "../Item/EquipItem/Weapon/WeaponItem.h"
#include "../Item/Escape/RadioEscapeZone.h"
#include "../Item/Escape/FlareEscapeZone.h"
#include "../Item/Escape/EscapeZone.h"
#include "../Item/Data/ItemSpawnLocation.h"
#include "../Character/Enemy/Enemy.h"
#include "../GameState/ExfilGameState.h"

USpawnManager::USpawnManager()
{
	mItemSpawnerClass = AItemSpawner::StaticClass();
	mEnemySpawnerClass = AEnemySpawner::StaticClass();
	mSupplyItemClass = ASupplyItem::StaticClass();
	mPickUpItemClass = AItemBase::StaticClass();
	mWeaponItemClass = AWeaponItem::StaticClass();
	mRadioEscapeZoneClass = ARadioEscapeZone::StaticClass();
	mFlareEscapeZoneClass = AFlareEscapeZone::StaticClass();

}

USpawnManager* USpawnManager::GetSpawnManager(const UObject* World)
{
	if (!IsValid(World)) return nullptr;

	return UGameplayStatics::GetGameInstance(World)->GetSubsystem<USpawnManager>();
}

//인벤토리에서 아이템 소환
AItemBase* USpawnManager::SpawnItemFromInventory(FVector spawnlocation)
{
	if (!mItemSpawner) return nullptr;
	if (!GetWorld() || GetWorld()->GetNetMode() == NM_Client) return nullptr;

	return mItemSpawner->SpawnItemFromInventory(mPickUpItemClass, spawnlocation);
}

//보급품 소환 함수
bool USpawnManager::SupplyItemSpawn()
{
	if (!SelectSupplyItemList()) return false;
	if (!mItemSpawner) return false;

	ASupplyItem* SupplyItem = nullptr;
	SupplyItem = mItemSpawner->SupplyItemSpawn(mSupplyItemClass);
	if (!SupplyItem) return false;

	SupplyItem->SetDrop(true);
	SupplyItem->SetSupplyItemList(mSupplyItemTag);

	return true;
}

//보급품 선택 함수
bool USpawnManager::SelectSupplyItemList()
{
	mSupplyCount = 0;

	int32 SelectIndex = 0;

	FSpawnData Data;

	UItemDataSubsystem* IDS = UItemDataSubsystem::GetInstance(GetWorld());
	if (!IDS) return false;

	while (true)
	{
		SelectIndex = FMath::RandRange(0, IDS->GetItemArrayNum()-1);
		
		Data.tag = FGameplayTag::RequestGameplayTag(IDS->GetItemName(SelectIndex));

		if (Data.tag.ToString().Contains(TEXT("Escape"))) continue;

		mSupplyItemTag.Add(Data.tag);
		++mSupplyCount;

		if (mSupplyCount > 2)
		{
			return true;
		}
	}

	return false;
}

void USpawnManager::GetAllSpawnLoaction(AGameMode* gm)
{

	if (!gm) return;

	UWorld* SW = gm->GetWorld();

	mItemSpawner = Cast<AItemSpawner>(UGameplayStatics::GetActorOfClass(SW, mItemSpawnerClass));
	//mEnemySpawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(SW, mEnemySpawnerClass));

	//if (!mItemSpawner || !mEnemySpawner) return false;

	TArray<AActor*>	SpawnLocation;
	UGameplayStatics::GetAllActorsOfClass(SW, AItemSpawnLocation::StaticClass(), SpawnLocation);
	for (AActor* location : SpawnLocation)
	{
		AItemSpawnLocation* ISL = Cast<AItemSpawnLocation>(location);
		if (!ISL) return;

		switch (ISL->mType)
		{
		case ESpawnType::None:
			break;
		case ESpawnType::Item:
			mItemSpawnLocations.Add(ISL->GetActorLocation());
			break;
		case ESpawnType::Radio:
			mRadioSpawnLocations.Add(ISL->GetActorLocation());
			break;
		case ESpawnType::Escape:
			SetRadioSpawnLocation(ISL->GetActorLocation());
			break;
		}
		ISL->Destroy();
	}
	return;
}

AWeaponItem* USpawnManager::SpawnWeaponItem()
{
	if (!mWeaponItemClass) return nullptr;

	return mItemSpawner->SpawnWeaponItem(mWeaponItemClass);
}

void USpawnManager::SpawnForGameStart(AGameMode* gm)
{
	RadioItemSpawn(gm->GetWorld());
	ItemSpawnForBeginPlay(gm->GetWorld());
	//EnemySpawnForBeginPlay(gm->GetWorld());
}

AActor* USpawnManager::SpawnRadioEscapeZone(FVector location)
{
	AEscapeZone* Zone = mItemSpawner->SpawnEscapeZone(mRadioEscapeZoneClass, location);
	if (!Zone) return nullptr;
	Zone->SetEscapeID(mEscapeZoneCount);
	++mEscapeZoneCount;

	return Zone;
}

AActor* USpawnManager::SpawnFlareEscapeZone(FVector location)
{
	FVector StartLocation = location + FVector(-30000.f, 0.f, 0.f);
	StartLocation.Z = 10000.f;

	AEscapeZone* Zone = mItemSpawner->SpawnEscapeZone(mFlareEscapeZoneClass, location);
	if (!Zone) return nullptr;
	Zone->SetEscapeID(mEscapeZoneCount);
	++mEscapeZoneCount;

	return Zone;
}

void USpawnManager::SpawnEnemyForEscapeEvent(int32 spawncount,AController* player)
{
	if (!mEnemySpawner) return;

	mEnemySpawner->SpawnEnemyForEscapeEvent(spawncount,player);
}

void USpawnManager::ItemSpawnForBeginPlay(UWorld* sw)
{
	if (!sw) return;

	UWorld* ServerWorld = sw;
	
	AItemBase* PickupItem;
	for (auto location : mItemSpawnLocations)
	{
		PickupItem = mItemSpawner->SpawnItemToWorld(ServerWorld, mPickUpItemClass,location);

		if (!PickupItem) return;
		FGameplayTag tag = UItemDataSubsystem::GetInstance(ServerWorld)->GetRandomItemSpawnData();
		if (tag.IsValid())
		{
			PickupItem->ItemSetting(tag);
		}
		else
		{
			PickupItem->Destroy();
		}

	}

}

void USpawnManager::EnemySpawnForBeginPlay(UWorld* world)
{
	if (!world) return;

	//AEnemy* Enemy;
	
	mEnemySpawner->TestSpawn(3);
	
}
//라디오 아이템 소환
void USpawnManager::RadioItemSpawn(UWorld* world)
{

	if (!world) return;
	int32 select = FMath::RandRange(0, mRadioSpawnLocations.Num()-1);

	FVector location = mRadioSpawnLocations[select];

	AItemBase* PickupItem;
	PickupItem = mItemSpawner->SpawnItemToWorld(world, mPickUpItemClass, location);
	if (!PickupItem) return;
	PickupItem->ItemSetting(FGameplayTag::RequestGameplayTag(TEXT("Item.Escape.Radio")));

	AItemBase* MapItem;
	location += FVector(100.f, 0, 0);
	MapItem = mItemSpawner->SpawnItemToWorld(world, mPickUpItemClass, location);
	if (!MapItem) return;

	MapItem->ItemSetting(FGameplayTag::RequestGameplayTag(TEXT("Item.Escape.Map")));
	mRadioSpawnLocations.Empty();
	return;
}

void USpawnManager::SetRadioSpawnLocation(FVector location)
{
	AExfilGameState* EGS = Cast<AExfilGameState>(GetWorld()->GetGameState());
	if (!EGS) return;

	EGS->SetRadioEscapeLocation(location);
}


void USpawnManager::SpawnerSetting(FString Spawnername, ASpawnerBase* Spawner)
{
	if (Spawnername.IsEmpty()) return;
	if (!IsValid(Spawner)) return;

	if (Spawnername == FString(TEXT("Item")))
	{
		AItemSpawner* IS = Cast<AItemSpawner>(Spawner);
		if (!IS) return;

		mItemSpawner = IS;
	}
	else if (Spawnername == FString(TEXT("Enemy")))
	{
		AEnemySpawner* ES = Cast<AEnemySpawner>(Spawner);
		if (!ES) return;
		mEnemySpawner = ES;
	}
}