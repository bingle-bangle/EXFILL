///////////////////////////////
/*
	생성날짜 : 25.09.14
	생 성 자 : 조소정
	기	  능 : 보급품 아이템 클래스
*/
///////////////////////////////

#pragma once

#include "EngineMinimal.h"
#include "ItemBase.h"
#include "../Component/Inventory/InventoryProvider.h"
#include "../Component/Inventory/InventoryComponent.h"
#include "SupplyItem.generated.h"


UCLASS()
class EXFIL_API ASupplyItem : public AItemBase
{
	GENERATED_BODY()


public:
	ASupplyItem();
	void SetSupplyItemList(TArray<FGameplayTag> ItemList);
	virtual void Tick(float DeltaTime) override;
	void	GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;
	void	SetDrop(bool State);


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated)
	TArray<FGameplayTag> mSupplyItemList;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent>   mTopMesh;

	bool bDrop = true;
	FHitResult DropResult;



public:

	// 열 때 락(Owner 세팅)
	UFUNCTION(BlueprintCallable)
	void LockTo(APlayerController* PC) { SetOwner(PC); }
	UFUNCTION(BlueprintCallable)
	void Unlock() { SetOwner(nullptr); }

	// 태그 리스트로 실제 아이템 채우기(서버에서만 호출)
	UFUNCTION(BlueprintCallable)
	void FillFromTags();

	UFUNCTION(BlueprintPure, Category = "Supply|Inventory")
	UInventoryComponent* GetInventory() const { return SupplyInventory; }

	UPROPERTY(Replicated)
	bool bFilledOnce = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> SupplyInventory;

};
