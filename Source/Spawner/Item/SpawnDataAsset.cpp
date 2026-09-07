// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnDataAsset.h"


USpawnDataAsset::USpawnDataAsset()
{

}

float USpawnDataAsset::RequestItemSpawnCount(FGameplayTag tag) const
{
	if (!tag.IsValid()) return -1;

	FName tagname = tag.GetTagName();

	float rate = *(mItemRates.Find(tagname));

	if (tagname.ToString().Contains(TEXT("Consum")))
	{
		return	rate* mConsumRate;
	}
	else if (tagname.ToString().Contains(TEXT("Weapon")))
	{
		return rate * mWeaponRate; 
	}
	else if(tagname.ToString().Contains(TEXT("Item.Escape")))
	{
		return rate * mMapRate;
	}
	
	return -1;
}

