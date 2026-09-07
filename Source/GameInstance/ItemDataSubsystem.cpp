// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataSubsystem.h"
#include "../Item/Data/ItemDataTable.h"
#include "Kismet/DataTableFunctionLibrary.h"



UItemDataSubsystem::UItemDataSubsystem()
{
	mItemDataTable = GetDefault<UItemDataTable>();

	static ConstructorHelpers::FObjectFinder<USpawnDataAsset> SD(TEXT("/Script/Exfil.SpawnDataAsset'/Game/Item/ItemData/Spawn/DA_Spawn.DA_Spawn'"));
	if (SD.Succeeded())
	{
		mSpawnDataAsset = SD.Object;
	}
}

UItemDataSubsystem* UItemDataSubsystem::GetInstance(const UObject* World)
{
	if (!World) return nullptr;

	return UGameplayStatics::GetGameInstance(World)->GetSubsystem<UItemDataSubsystem>();
}


void UItemDataSubsystem::SetItemSpawnTable(int32 allplayercount)
{
	
	UDataTableFunctionLibrary::GetDataTableRowNames(mItemDataTable->GetDataTable(), mItemSpawnTables);
	//mAllItemCount = allplayercount * 0.8;
	mAllItemCount = 80;
	float rate = 0;

	mSpawnItemRates.Init(FSpawnRate(), mItemSpawnTables.Num() - 1);

	for (int32 idx=0; idx < mItemSpawnTables.Num(); idx++)
	{

		FGameplayTag tag = FGameplayTag::RequestGameplayTag(mItemSpawnTables[idx]);
		if (!tag.IsValid()) return;

		if (tag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Item.Escape.Radio")))) continue;

		rate = mSpawnDataAsset->RequestItemSpawnCount(tag);

		if (rate < 0) continue;

		rate =FMath::RoundToInt32(rate*mAllItemCount);
		
		FSpawnRate Rate;
		Rate.Count = rate;
		Rate.Tag = tag;
		mSpawnItemRates.Add(Rate);

	}
}

FGameplayTag UItemDataSubsystem::GetRandomItemSpawnData()
{
	if (mSpawnItemRates.Num() == 0) return FGameplayTag();

	for (int32 idx = 0; idx < mAllItemCount; ++idx)
	{
		float pick = FMath::RandRange(0.f, 1.f);
		float compare = 0.f;
		for (int32 ridx =0; ridx<mSpawnItemRates.Num(); ++ridx)
		{
			if (mSpawnItemRates[ridx].Count == 0)
			{ 
				mSpawnItemRates.RemoveAtSwap(ridx);
				continue;
			}
			float rate = static_cast<float>(mSpawnItemRates[ridx].Count) /static_cast<float>(mAllItemCount);
			compare += rate;
			if (pick <= compare)
			{
				mAllItemCount--;
				mSpawnItemRates[ridx].Count--;
				return mSpawnItemRates[ridx].Tag;
			}
			else
			{
				compare+=rate;
			}
		}
	}

	return FGameplayTag();
}

int32 UItemDataSubsystem::GetItemArrayNum() const
{

	 return mItemSpawnTables.Num();
}

bool UItemDataSubsystem::CheckLastIndex(int32 index)
{
	if(mItemSpawnTables.Num()<index) return false;

	return true;
}

FName UItemDataSubsystem::GetItemName(int32 idx) const
{
	if (idx < 0) return FName();
	return mItemSpawnTables[idx];
}

FDataForItemEffect* UItemDataSubsystem::GetItemEffect(FGameplayTag ItemTag) const
{
	if (!ItemTag.IsValid()) return nullptr;

	return mItemDataTable->GetItemEffect(ItemTag);
}

FDataForWeapon* UItemDataSubsystem::GetWeaponData(FGameplayTag tag) const
{
	if (!tag.IsValid()) return nullptr;

	return mItemDataTable->GetWeaponData(tag);
}

UItemDataAsset* UItemDataSubsystem::GetItemDataAsset(FGameplayTag tag) const
{
	if (!tag.IsValid()) return nullptr;

	return mItemDataTable->GetItemDataAsset(tag.GetTagName());
}
