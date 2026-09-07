// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Spawner/Item/SpawnDataAsset.h"
#include "ItemDataSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FSpawnRate
{
	GENERATED_BODY()
public:
	FGameplayTag Tag;
	int32		Count=0;
};

class UItemDataAsset;
class UItemDataTable;
class USpawnDataAsset;
UCLASS()
class EXFIL_API UItemDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
	UItemDataSubsystem();


public:
	static UItemDataSubsystem* GetInstance(const UObject* World);

	void SetItemSpawnTable(int32 allplayercount);
	FGameplayTag GetRandomItemSpawnData();
	int32 GetItemArrayNum() const;
	bool CheckLastIndex(int32 index);

	FName	GetItemName(int32 idx) const;

	FDataForItemEffect* GetItemEffect(FGameplayTag tag) const;
	FDataForWeapon* GetWeaponData(FGameplayTag tag) const;
	UItemDataAsset* GetItemDataAsset(FGameplayTag tag) const;

protected:	
	UPROPERTY()
	const UItemDataTable* mItemDataTable;
	UPROPERTY()
	const USpawnDataAsset*  mSpawnDataAsset;

	TArray<FName>  mItemSpawnTables;
	int32 mAllItemCount = 0;
	TArray<FSpawnRate>  mSpawnItemRates;
};
