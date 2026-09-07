///////////////////////////////
/*
	생성날짜 : 25.09.03
	생성자 : 조소정
	기능 : 게임 진행 중 아이템을 스폰할 때 사용할 수 있는 클래스

*/
///////////////////////////////
#pragma once

#include "EngineMinimal.h"
#include "../SpawnerBase.h"
#include "ItemSpawner.generated.h"


class AItemBase;
class ASupplyItem;
class AEscapeZone;
class AWeaponItem;

UCLASS()
class EXFIL_API AItemSpawner : public ASpawnerBase
{
	GENERATED_BODY()


public:
	AItemSpawner();


public:
	virtual void BeginPlay() override;


	AItemBase* SpawnItemFromInventory(TSubclassOf<AItemBase> pickupitemclass, FVector spawnLocation = FVector(0, 0, 0));
	ASupplyItem* SupplyItemSpawn(TSubclassOf<ASupplyItem> supplyitemclass);
	AItemBase*   SpawnItemToWorld(UWorld* world,TSubclassOf<AItemBase>  ItemClass, FVector SpawnLocation = FVector(0, 0, 0));
	AEscapeZone* SpawnEscapeZone(TSubclassOf<AEscapeZone> escapeclass, FVector location);

	AWeaponItem* SpawnWeaponItem(TSubclassOf<AWeaponItem> WeaponClass);

protected:
	TObjectPtr<class AItemBase>	mSpawnItem;

private:
	UWorld* GetServerWorld();


	TArray<FName>	mDataTableRowKeies;
	int32			mSelectIndex = 0;
};
