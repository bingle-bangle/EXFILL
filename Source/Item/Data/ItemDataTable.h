///////////////////////////////
/*
	생성일자 : 25.09.014
	생 성 자 : 조소정
	기능 : 아이템 정보를 저장해두는 클래스

*/
///////////////////////////////
#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataTable.generated.h"

class UItemDataAsset;

USTRUCT(BlueprintType)
struct FItemTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UItemDataAsset* DataAsset;
	UPROPERTY(EditAnywhere)
	UStaticMesh* STM = nullptr;
	UPROPERTY(EditAnywhere)
	FDataForWeapon DataForWeapon;
	UPROPERTY(EditAnywhere)
	FDataForItemEffect	DataForEffect;
};

UCLASS()
class EXFIL_API UItemDataTable : public UObject
{
	GENERATED_BODY()

	
public:
	UItemDataTable();

public:
	FItemTable*         GetItemData(FGameplayTag ItemTag) const;
	FDataForWeapon*     GetWeaponData(FGameplayTag ItemTag) const;
	FDataForItemEffect* GetItemEffect(FGameplayTag ItemTag) const;
	FName		        GetItemName(FGameplayTag ItemTag) const;
	UItemDataAsset*     GetItemDataAsset(FName ItemName) const;
	UItemDataAsset*		GetItemDataAssetByTag(FGameplayTag ItemTag) const;
	int32		        GetItemValue(FGameplayTag ItemTag) const;
	UDataTable*         GetDataTable() const;
	FVector				GetItemScale(FName itemname) const;
	FRotator			GetItemRotation(FName itemname) const;

private:
	TObjectPtr<UDataTable>		mItemDataTable;

	
};
