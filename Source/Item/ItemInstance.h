//////////////////////////
/*
    생성 날짜 : 2025.09.12
    생성자 : 김다해

    기능 :
      - 인벤토리 아이템 단일 개체 상태(데이터 에셋 참조 + 개체별 가변 상태)
*/
//////////////////////////


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemInstance.generated.h"

class UItemDataAsset;


UCLASS(BlueprintType)
class EXFIL_API UItemInstance : public UObject
{
    GENERATED_BODY()

public:
    //데이터 에셋을 기반으로 이 인스턴스를 초기화하기
    void InitializeFromDataAsset(UItemDataAsset* DataAsset);


    UFUNCTION(BlueprintPure, Category = "Item")
    UItemDataAsset* GetDataAsset() const;

    // --- 데이터를 읽어오는 함수들 (Getter) ---
    UFUNCTION(BlueprintCallable, Category = "Item")
    FIntPoint GetDimensions() const;

    UFUNCTION(BlueprintPure, Category = "Item")
    FName GetDisplayName() const;

    UFUNCTION(BlueprintPure, Category = "Item")
    int32 GetMaxStack() const;

    UFUNCTION(BlueprintPure, Category = "Item")
    int32 GetQuantity() const;

    UFUNCTION(BlueprintPure, Category = "Item")
    int32 GetDurability() const;


    UFUNCTION(BlueprintPure, Category = "Item")
    bool IsRotated() const;


    // --- 데이터를 변경하는 함수들 (Setter) ---
    UFUNCTION(BlueprintCallable, Category = "Item")
    void SetRotated(bool bNewRotated);

    UFUNCTION(BlueprintCallable, Category = "Item")
    void SetCount(int32 NewCount);

    UFUNCTION(BlueprintCallable, Category = "Item")
    void AddQuantity(int32 AmountToAdd);

    void SetData(UItemDataAsset* NewData);


    UFUNCTION(BlueprintCallable, Category = "Item|Init")
    bool InitializeFromTag(const UObject* WorldContext, FGameplayTag ItemTag, int32 Count = 1, bool bRotated = false);

    UFUNCTION(BlueprintCallable, Category = "Item|Factory")
    static UItemInstance* CreateFromTag(const UObject* WorldContext, UObject* Outer, FGameplayTag ItemTag, int32 Count = 1, bool bRotated = false);


protected:
    // 기본 데이터(프로토타입)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    TObjectPtr<UItemDataAsset> mData = nullptr;

    // 스택 수량
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    int32 mCount = 1;

    // 회전 여부(가로/세로 교환)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    bool mRotated = false;

    // 아이템의 가변적인 상태 (예: 내구도 등)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    int32 mDurability = 0;

};
