/*
    생성일자 : 25.09.02
    생 성 자 : 조소정
    기능 : 아이템에 쓰는 데이터에셋

*/

#pragma once

#include "../GameInfo.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FDataForInven
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FGameplayTag	mTag;
    UPROPERTY(EditAnywhere)
    FName	        mItemName;
    UPROPERTY(EditAnywhere)
    FString		    mItemDescription;
    UPROPERTY(EditAnywhere)
    int32	        mItemValue = 0;
    UPROPERTY(EditAnywhere)
    int32		    mMaxStack = 0;
    UPROPERTY(EditAnywhere)
    float		    mCoolDown = 0.f;
    UPROPERTY(EditAnywhere)
    FIntPoint       mSizeInTiles = FIntPoint::ZeroValue;
    UPROPERTY(EditAnywhere)
    TObjectPtr<UTexture2D>  mIcon;
    UPROPERTY(EditAnywhere)
    double     mScale = 0.0;
    UPROPERTY(EditAnywhere)
    FRotator    mRotator = FRotator::ZeroRotator;
};



UCLASS()
class EXFIL_API UItemDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UItemDataAsset();

    FName               GetItemName() const;
    UFUNCTION(BlueprintPure)
    UTexture2D*    GetItemIconTexture() const;
    const FString& GetItemDescription() const;
    int32               GetItemValue() const;
    FIntPoint           GetSizeInTiles() const;
    int32               GetMaxStack() const;
    float               GetCoolDown() const;
    FGameplayTag        GetItemTag()    const;
    FGameplayTag	    NameToTag(FName Name) const;
    FName               TagToName(FGameplayTag Tag) const;
    double              GetScaleforSpawn() const;
    FRotator            GetRotatorforSpawn() const;

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSubclassOf<class AItemBase> ItemActorClass;
    UFUNCTION(BlueprintPure, Category = "Spawn")
    TSubclassOf<class AItemBase> GetItemActorClass() const;*/

private:
    UPROPERTY(EditAnywhere, Category = "ItemSetting")
    FDataForInven        mItemInfo;


};
