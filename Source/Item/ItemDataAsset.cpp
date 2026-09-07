#include "ItemDataAsset.h"
#include "ItemBase.h"

UItemDataAsset::UItemDataAsset() {}

FName UItemDataAsset::GetItemName() const
{
    return mItemInfo.mItemName;
}


UTexture2D* UItemDataAsset::GetItemIconTexture() const
{

    return mItemInfo.mIcon;
}

const FString& UItemDataAsset::GetItemDescription() const
{
    return mItemInfo.mItemDescription;
}

int32 UItemDataAsset::GetItemValue() const
{
    return mItemInfo.mItemValue;
}

FIntPoint UItemDataAsset::GetSizeInTiles() const
{
    return mItemInfo.mSizeInTiles;
}

int32 UItemDataAsset::GetMaxStack() const
{
    return mItemInfo.mMaxStack;
}

float UItemDataAsset::GetCoolDown() const
{
    return mItemInfo.mCoolDown;
}

FGameplayTag UItemDataAsset::GetItemTag() const
{
    return mItemInfo.mTag;
}

FGameplayTag UItemDataAsset::NameToTag(FName Name) const
{
    return FGameplayTag::RequestGameplayTag(Name, false);
}

FName UItemDataAsset::TagToName(FGameplayTag Tag) const
{
    return Tag.GetTagName();
}

double UItemDataAsset::GetScaleforSpawn() const
{
    return mItemInfo.mScale;
}

FRotator UItemDataAsset::GetRotatorforSpawn() const
{
    return mItemInfo.mRotator;
}

