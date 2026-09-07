// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Engine/DataAsset.h"
#include "SpawnDataAsset.generated.h"



UCLASS()
class EXFIL_API USpawnDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USpawnDataAsset();

	float RequestItemSpawnCount(FGameplayTag tag) const;

protected:
	UPROPERTY(EditAnywhere)
	TMap<FName, float>  mItemRates;

	UPROPERTY(EditAnywhere)
	float mConsumRate;
	UPROPERTY(EditAnywhere)
	float mWeaponRate;
	UPROPERTY(EditAnywhere)
	float mMapRate;
	

};
