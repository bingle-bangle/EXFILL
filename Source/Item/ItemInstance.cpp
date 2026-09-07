#include "ItemInstance.h"
#include "ItemDataAsset.h"
#include "Data/ItemDataTable.h"
#include "../GameInstance/ItemDataSubsystem.h"

void UItemInstance::InitializeFromDataAsset(UItemDataAsset* DataAsset)
{
	if (DataAsset)
	{
		mData = DataAsset;

		mCount = 1;

		//  데이터 에셋에 정의된 기본 값(탄창 갯수)로 초기화하기
		//mDurability = mData->GetDefaultDurability();
	}
}

FIntPoint UItemInstance::GetDimensions() const
{
	if (!IsValid(mData))
	{
		return FIntPoint(1, 1);
	}
	FIntPoint Dimensions = mData->GetSizeInTiles();
	if (mRotated)
	{
		Swap(Dimensions.X, Dimensions.Y);
	}
	return Dimensions;

}

FName UItemInstance::GetDisplayName() const
{
	if (mData != nullptr)
	{
		return mData->GetItemName();
	}
	else
	{
		return NAME_None;
	}
}

int32 UItemInstance::GetMaxStack() const
{
	if (mData != nullptr)
	{
		return mData->GetMaxStack();
	}
	else
	{
		return 1;
	}
}

UItemDataAsset* UItemInstance::GetDataAsset() const
{
	return mData;
}

int32 UItemInstance::GetQuantity() const
{
	return mCount;
}

int32 UItemInstance::GetDurability() const
{
	return mDurability;
}

bool UItemInstance::IsRotated() const
{
	return mRotated;
}

void UItemInstance::AddQuantity(int32 AmountToAdd)
{
	// 현재 수량에 AmountToAdd만큼 더하기
	mCount += AmountToAdd;
}



void UItemInstance::SetRotated(bool bNewRotated)
{
	mRotated = bNewRotated;
}

void UItemInstance::SetCount(int32 NewCount)
{
	if (NewCount < 0)
	{
		mCount = 0;
	}
	else
	{
		mCount = NewCount;
	}
}

void UItemInstance::SetData(UItemDataAsset* NewData)
{
	mData = NewData;
}

bool UItemInstance::InitializeFromTag(const UObject* WorldContext, FGameplayTag ItemTag, int32 Count, bool bRotated)
{
	UItemDataAsset* Data = UItemDataSubsystem::GetInstance(WorldContext)->GetItemDataAsset(ItemTag);
	if (!Data) return false;

	SetData(Data);
	SetCount(FMath::Max(1, Count));
	SetRotated(bRotated);
	return true;
}

UItemInstance* UItemInstance::CreateFromTag(const UObject* WorldContext, UObject* Outer, FGameplayTag ItemTag, int32 Count, bool bRotated)
{
	if (!Outer) return nullptr;

	UItemDataAsset* Data = UItemDataSubsystem::GetInstance(WorldContext)->GetItemDataAsset(ItemTag);
	if (!Data) return nullptr;

	UItemInstance* Inst = NewObject<UItemInstance>(Outer);
	if (!Inst) return nullptr;

	Inst->SetData(Data);
	Inst->SetCount(FMath::Max(1, Count));
	Inst->SetRotated(bRotated);
	return Inst;
}