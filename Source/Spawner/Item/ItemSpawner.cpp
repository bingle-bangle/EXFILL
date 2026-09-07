
#include "ItemSpawner.h"
#include "../../Item/ItemBase.h"
#include "../../Item/SupplyItem.h"
#include "../../GameInstance/SpawnManager.h"
#include "../../Item/Escape/EscapeZone.h"
#include "../../Item/EquipItem/Weapon/WeaponItem.h"

AItemSpawner::AItemSpawner()
{
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;

	USpawnManager::GetSpawnManager(GetWorld())->SpawnerSetting(FString(TEXT("Item")), this);
}

//월드에 아이템을 소환하는 함수
AItemBase* AItemSpawner::SpawnItemFromInventory(TSubclassOf<AItemBase> pickupitemclass, FVector spawnLocation)
{
	if (!pickupitemclass) return nullptr;

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform spawntransform;
	spawntransform.SetLocation(spawnLocation);
	spawntransform.SetScale3D(FVector(1.5f));
	mSpawnItem = GetWorld()->SpawnActor<AItemBase>(pickupitemclass, spawntransform, param);
	
	return mSpawnItem;
}

//보급품 아이템을 스폰 함수
ASupplyItem* AItemSpawner::SupplyItemSpawn(TSubclassOf<ASupplyItem> supplyitemclass)
{
	if (!HasAuthority()) return nullptr;
	if (!supplyitemclass) return nullptr;
	FVector SpawnPoint = GetActorLocation();
	SpawnPoint.Z += 800;
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	return GetWorld()->SpawnActor<ASupplyItem>(supplyitemclass, SpawnPoint, FRotator::ZeroRotator, param);

}

AItemBase* AItemSpawner::SpawnItemToWorld(UWorld* world,TSubclassOf<AItemBase> ItemClass, FVector SpawnLocation)
{
	if (!HasAuthority() || !world)
		return nullptr;

	UWorld* ServerWorld = GetServerWorld();

	UNetDriver* NetDriver = world->GetNetDriver();
	bool bIsServer = (NetDriver && NetDriver->IsServer());
	
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform spawntransform;
	spawntransform.SetLocation(SpawnLocation);
	//spawntransform.SetScale3D(FVector(1.5f));
	mSpawnItem = world->SpawnActor<AItemBase>(ItemClass, spawntransform, param);

	if (mSpawnItem) return mSpawnItem;


	return nullptr;
}

AEscapeZone* AItemSpawner::SpawnEscapeZone(TSubclassOf<AEscapeZone> escapeclass, FVector location)
{
	if (!escapeclass) return nullptr;
	if (!HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Yellow, TEXT("No Authority"));
		return nullptr;
	}
	FVector SpawnPoint = location;

	return GetWorld()->SpawnActor<AEscapeZone>(escapeclass, SpawnPoint, FRotator::ZeroRotator);
}

AWeaponItem* AItemSpawner::SpawnWeaponItem(TSubclassOf<AWeaponItem> WeaponClass)
{
	if (!WeaponClass) return nullptr;

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeaponItem* Weapon = GetWorld()->SpawnActor<AWeaponItem>(WeaponClass, params);
	Weapon->SetActorScale3D(FVector(1.5f));
	return Weapon;
}

UWorld* AItemSpawner::GetServerWorld()
{
	if (GEngine)
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.World() && Context.World()->GetNetMode() == NM_ListenServer)
			{
				return Context.World(); 
			}
		}
	}
		return nullptr;
	
}