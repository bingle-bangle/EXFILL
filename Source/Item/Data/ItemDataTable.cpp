
#include "ItemDataTable.h"
#include "../ItemDataAsset.h"

UItemDataTable::UItemDataTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>   DT(TEXT("/Script/Engine.DataTable'/Game/Item/ItemData/DT_ItemDataTable.DT_ItemDataTable'"));
	if (DT.Succeeded())
		mItemDataTable = DT.Object;
	
}

//데이터 테이블의 한 행에 있는 모든 정보를 가져오는 함수
FItemTable* UItemDataTable::GetItemData(FGameplayTag ItemTag) const
{
	FName ItemName = ItemTag.GetTagName();
	FItemTable* Table = mItemDataTable->FindRow<FItemTable>(ItemName, TEXT(""));
	return Table;
}

//무기 장착시 필요한 데이터를 가져오는 함수
FDataForWeapon* UItemDataTable::GetWeaponData(FGameplayTag ItemTag) const
{
	FName ItemName = ItemTag.GetTagName();
	FItemTable* Row = mItemDataTable->FindRow<FItemTable>(ItemName, TEXT(""));

	if (Row)
	{
		return &Row->DataForWeapon;
	}

	return nullptr;
}
//아이템 사용시 필요한 데이터를 가져오는 함수
FDataForItemEffect* UItemDataTable::GetItemEffect(FGameplayTag ItemTag) const
{
	FName ItemName = ItemTag.GetTagName();
	FItemTable* Row = mItemDataTable->FindRow<FItemTable>(ItemName, TEXT(""));

	if (Row)
	{
		return &Row->DataForEffect;
	}
	
	return nullptr;
}

//Tag로 아이템 정보를 가져오는 함수
FName UItemDataTable::GetItemName(FGameplayTag ItemTag) const
{
	if (!mItemDataTable) return FName();


	FName ItemName = ItemTag.GetTagName();
	FItemTable* Table = mItemDataTable->FindRow<FItemTable>(ItemName, TEXT(""));

	return Table->DataAsset->GetItemName();
}

//인벤토리에 필요한 데이터를 가져오는 함수
UItemDataAsset* UItemDataTable::GetItemDataAsset(FName ItemName) const
{
	FItemTable* Table = mItemDataTable->FindRow<FItemTable>(ItemName, TEXT(""));

	return Table->DataAsset;
}

//각 아이템의 수치 값을 가져오는 함수
int32 UItemDataTable::GetItemValue(FGameplayTag ItemTag) const
{
	if (!mItemDataTable) return 0;


	FName ItemName = ItemTag.GetTagName();
	FItemTable* Table = mItemDataTable->FindRow<FItemTable>(ItemName, TEXT(""));

	return Table->DataAsset->GetItemValue();
}

//전체 데이터 테이블을 가져오는 함수
UDataTable* UItemDataTable::GetDataTable() const
{
	if(!mItemDataTable) return nullptr;

	return mItemDataTable;
}

UItemDataAsset* UItemDataTable::GetItemDataAssetByTag(FGameplayTag ItemTag) const
{
	if (!mItemDataTable) return nullptr;
	if (FItemTable* Row = mItemDataTable->FindRow<FItemTable>(ItemTag.GetTagName(), TEXT("GetItemDataAssetByTag")))
	{
		return Row->DataAsset;
	}
	return nullptr;
}

FRotator UItemDataTable::GetItemRotation(FName itemname) const
{
	FItemTable* Table = mItemDataTable->FindRow<FItemTable>(itemname, TEXT(""));

	return Table->DataAsset->GetRotatorforSpawn();
}

FVector UItemDataTable::GetItemScale(FName itemname) const
{
	FItemTable* Table = mItemDataTable->FindRow<FItemTable>(itemname, TEXT(""));
	
	double scale = Table->DataAsset->GetScaleforSpawn();

	return FVector(scale, scale, scale);
}
