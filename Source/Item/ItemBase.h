/*

생성날짜 : 2025.08.27
생성자 : 조소정
기능 : 아이템 베이스를 위한 클래스


*/
#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ItemBase.generated.h"

class UItemDataTable;
class UItemDataAsset;
class UItemPickUpWidget;

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UStaticMesh> Mesh;
	UPROPERTY()
	FGameplayTag Tag;
	UPROPERTY()
	FName ItemName;
	UPROPERTY()
	FRotator  ItemRoatation = FRotator::ZeroRotator;
	UPROPERTY()
	FVector      Scale = FVector::ZeroVector;
	UPROPERTY()
	int32		Value = 0;
};


UCLASS()
class EXFIL_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	AItemBase();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SwitchView(bool State);
	void ItemSetting(FGameplayTag Tag, int32 ItemValue = 0);
	void ItemSetting();
	void SetItemTable(UItemDataTable* Table);
	

	UFUNCTION()
	void OnRep_ItemData();

	FGameplayTag  GetGameplayTag() const;
	UItemDataAsset* GetItemDataAsset() const;
	FString GetItemName() const;


protected:
	const UItemDataTable* mItemDataTable;

	UPROPERTY(ReplicatedUsing = OnRep_ItemData)
	FItemData  mItemData;
	
	TObjectPtr <USphereComponent> mCollision;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent>	mMesh;
	UPROPERTY(EditAnywhere)
	FGameplayTag			mTag;
	FName					mItemName;
	int32					mItemValue;

	TObjectPtr<UMaterial>	mOverlayMaterial;

	virtual void BeginPlay() override;

	//add 김다해
	UPROPERTY(Replicated)
	int32 mStackCount = 1;

public:
	int32 GetStackCount() const { return FMath::Max(1, mStackCount); }
	void  SetStackCount(int32 NewCount) { mStackCount = FMath::Max(1, NewCount); }
};