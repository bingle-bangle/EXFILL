
//TODO : 위젯과 연결해서 보급품 상자를 열었을 때
//아이템 아이콘이 나와야하고 마우스를 갖다대면 아이템 정보가 나와야함.

#include "SupplyItem.h"
#include "../Component/Inventory/InventoryComponent.h"
#include "../Item/ItemDataAsset.h"
#include "../GameInstance/ItemDataSubsystem.h"
#include "../Item/Data/ItemDataTable.h"


ASupplyItem::ASupplyItem()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/AirDrop/Meshes/SM_AirDropBody.SM_AirDropBody'"));
	if (Mesh.Succeeded())
		mMesh->SetStaticMesh(Mesh.Object);

	mCollision->SetCollisionProfileName(TEXT("SupplyItem"));
	mMesh->SetRelativeLocation(FVector(0, 0, -110.f));

	mTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mTopMesh"));
	mTopMesh->SetupAttachment(mMesh);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TopMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/AirDrop/Meshes/SM_AirDropParachute.SM_AirDropParachute'"));
	if (TopMesh.Succeeded())
		mTopMesh->SetStaticMesh(TopMesh.Object);

	mItemName = FName(TEXT("보급품"));
	mCollision->SetSphereRadius(120.f);

	// kdh : add
	bReplicates = true;
	SupplyInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("SupplyInventory"));
	SupplyInventory->SetIsReplicated(true);
	SupplyInventory->mColumns = 6;
	SupplyInventory->mRows = 4;
	SupplyInventory->mTileSize = 64.f;
	SupplyInventory->bReplicateToAllClients = true;
	//

	mSupplyItemList.Init(FGameplayTag(), 3);
}
void ASupplyItem::BeginPlay()
{
	Super::BeginPlay();

}

void ASupplyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDrop)
	{
		AddActorWorldOffset(FVector(0, 0, -5.f), true, &DropResult);
		if (DropResult.bBlockingHit)
		{
			bDrop = false;
			mTopMesh->SetStaticMesh(nullptr);
		}
	}

}

void ASupplyItem::SetSupplyItemList(TArray<FGameplayTag> ItemList)
{
	if (ItemList.IsEmpty())	return;
	
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("Num : %d"), ItemList.Num()));
	for (int32 i = 0; i < 3; ++i)
	{
		mSupplyItemList[i] = ItemList[i];
	}

	mItemData.ItemName = FName(TEXT("보급품"));
}



void ASupplyItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASupplyItem, mSupplyItemList);
}

void ASupplyItem::SetDrop(bool State)
{
	bDrop = State;
}

// kdh : add
void ASupplyItem::FillFromTags()
{
	if (!HasAuthority() || !SupplyInventory) return;

	if (bFilledOnce) return;

	int32 Added = 0;
	for (const FGameplayTag& Tag : mSupplyItemList)
	{
		if (!Tag.IsValid()) continue;
		if (UItemDataAsset* DA = UItemDataSubsystem::GetInstance(this)->GetItemDataAsset(Tag))
		{
			const bool bOk = SupplyInventory->TryAddItem(DA, 1, true);
			if (bOk) ++Added;
		}
	}

	bFilledOnce = true;
}