/*
	생성날짜 : 25.09.10
	생 성 자 : 조소정
	기    능 : 장비칸에 착용하는 아이템을 위한 클래스

*/
#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "EquipItemBase.generated.h"

class  UItemDataAsset;

UCLASS()
class EXFIL_API AEquipItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEquipItemBase();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent>	mMesh;
	
	UPROPERTY(Replicated)
	FName			mSocketHand;

	UPROPERTY(Replicated)
	FName			mSocketHolster;
	
	UPROPERTY(Replicated)
	int32			mAttackValue;
	
	UPROPERTY(Replicated)
	FGameplayTag	mTag;

	UPROPERTY(ReplicatedUsing=OnRep_ChangeMesh)
	USkeletalMesh* mMeshAsset;

public:
	USkeletalMeshComponent* GetMesh() const
	{
		return mMesh;
	}

	FName GetSocketHand() const
	{
		return mSocketHand;
	}

	FName GetSocketHolster() const
	{
		return mSocketHolster;
	}

	int32 GetValue() const
	{
		return mAttackValue;
	}

	FGameplayTag GetItemTag() const
	{
		return mTag;
	}

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetItemSetting(FGameplayTag ItemData);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_ChangeMesh();

};
