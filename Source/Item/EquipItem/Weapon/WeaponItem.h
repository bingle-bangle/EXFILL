// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../EquipItemBase.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class EXFIL_API AWeaponItem : public AEquipItemBase
{
	GENERATED_BODY()
	
public:
	void SetEquipWeaponType(EWeaponType type);

private:
	EWeaponType  mWeaponType = EWeaponType::None;
	
};
