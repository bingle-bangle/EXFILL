///////////////////////////////
/*

	생성날짜 : 25.09.25
	생 성 자 : 조소정

	기    능 : 스폰 총괄 클래스

*/
///////////////////////////////
#pragma once

#include "../GameInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <GameplayTagContainer.h>

#include "SpawnManager.generated.h"

class AItemSpawnLocation;

class AGameMode;
USTRUCT(BlueprintType)
struct FSpawnType
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FGameplayTag ItemTag;
	UPROPERTY()
	int32	SpawnCount = 0;
};

class AItemSpawnLocation;


UCLASS()
class EXFIL_API USpawnManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
	USpawnManager();


public:
	static USpawnManager* GetSpawnManager(const UObject* World);

	void SpawnerSetting(FString Spawnername, class ASpawnerBase* Spawner);
	void SetItemSpawnCountAndTag();


	//적 스포너

	//아이템스폰
	bool SupplyItemSpawn();
	bool SelectSupplyItemList();
	void GetAllSpawnLoaction(AGameMode* gm);
	class AItemBase* SpawnItemFromInventory(FVector spawnLocation);
	class AWeaponItem* SpawnWeaponItem();
	void SpawnForGameStart(AGameMode* gm);

	//탈출이벤트 관련 스폰
	AActor* SpawnRadioEscapeZone(FVector location);
	AActor* SpawnFlareEscapeZone(FVector location);
	void SpawnEnemyForEscapeEvent(int32 Spawncount, AController* player);

private:
	
	void ItemSpawnForBeginPlay(UWorld* sw);
	void EnemySpawnForBeginPlay(UWorld* world);
	void RadioItemSpawn(UWorld* world);
	void SetRadioSpawnLocation(FVector location);
	bool SetItemSpawnData();
	void SetRadioEscapeEnable();

	TSubclassOf<class AItemSpawner>      mItemSpawnerClass;
	TSubclassOf<class AEnemySpawner>     mEnemySpawnerClass;
	TSubclassOf<class AItemBase>	     mPickUpItemClass;
	TSubclassOf<class ASupplyItem>       mSupplyItemClass;
	TSubclassOf<class AWeaponItem>	     mWeaponItemClass;
	TSubclassOf<class ARadioEscapeZone>  mRadioEscapeZoneClass;
	TSubclassOf<class AFlareEscapeZone>	 mFlareEscapeZoneClass;

	class AItemSpawner* mItemSpawner;
	class AEnemySpawner* mEnemySpawner;

	int32  mRadioLocationIndex = 0;
	TArray<FVector>   mRadioSpawnLocations;

	int32 mSupplyCount = 0;
	TArray<FGameplayTag>	mSupplyItemTag;

	int32 mEscapeZoneCount = 0;


	TArray<FVector>   mItemSpawnLocations;
	TArray<FVector>	  mEnemySpawnLocation;
};


