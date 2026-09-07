
#include "ItemBase.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Component/InteractComponent.h"
#include "../Item/Data/ItemDataTable.h"
#include "../UI/Item/ItemPickUpWidget.h"
#include "../GameMode/ExfilGameMode.h"


AItemBase::AItemBase()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	PrimaryActorTick.bCanEverTick = false;

	mCollision = CreateDefaultSubobject<USphereComponent>(TEXT("mCollision"));
	mCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mCollision->SetCollisionProfileName(FName("Item"));
	SetRootComponent(mCollision);
	mCollision->SetSphereRadius(32.f);

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mMesh"));
	mMesh->SetupAttachment(mCollision);
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mMesh->SetRelativeLocation(FVector(0,0,-30));
	mMesh->SetIsReplicated(true);


	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Game/Item/Effect/HigthLigth.HigthLigth'"));
	if (Material.Succeeded())
		mOverlayMaterial = Material.Object;

}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, mItemData);

	//add 김다해
	DOREPLIFETIME(AItemBase, mStackCount);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	mItemDataTable = GetDefault<UItemDataTable>();

	if (mTag.IsValid())
	{
		//ItemSetting();
	}
}

void AItemBase::SwitchView(bool State)
{
	if (State)
	{
		mMesh->SetOverlayMaterial(mOverlayMaterial);
	}
	else
	{
		mMesh->SetOverlayMaterial(nullptr);
	}
}

//태그를 넣으면 아이템 정보를 세팅해주는 함수
//값에 변화가 생겼을 때 넣어주면 값까지 다시 세팅
void AItemBase::ItemSetting(FGameplayTag Tag, int32 ItemValue)
{
	if (!HasAuthority()) return;

	mTag = Tag;
	if (!mItemDataTable) return;
	FString TagName = mTag.ToString();

	FItemTable* Table = mItemDataTable->GetItemData(mTag);
	mItemName = mItemDataTable->GetItemName(mTag);
	mMesh->SetStaticMesh(Table->STM);
	mMesh->SetRelativeRotation(mItemDataTable->GetItemRotation(mTag.GetTagName()));
	mMesh->SetRelativeScale3D(mItemDataTable->GetItemScale(mTag.GetTagName()));


	mItemData.Mesh = Table->STM;
	mItemData.ItemName = mItemName;
	mItemData.Tag = mTag;
	mItemData.ItemRoatation = mItemDataTable->GetItemRotation(mTag.GetTagName());
	mItemData.Scale = mItemDataTable->GetItemScale(mTag.GetTagName());


	if (ItemValue != 0)
	{
		mItemValue = ItemValue;
		mItemData.Value = ItemValue;
	}
	else
	{
		mItemValue = mItemDataTable->GetItemValue(mTag);
		mItemData.Value = mItemValue;
	}

}
void AItemBase::ItemSetting()
{
	if (!mItemDataTable) return;
	FString TagName = mTag.ToString();

	FItemTable* Table = mItemDataTable->GetItemData(mTag);
	mItemName = mItemDataTable->GetItemName(mTag);
	mMesh->SetStaticMesh(Table->STM);
	mItemValue = mItemDataTable->GetItemValue(mTag);

	mItemData.Mesh = Table->STM;
	mItemData.ItemName = mItemName;
	mItemData.Tag = mTag;
}

void AItemBase::SetItemTable(UItemDataTable* Table)
{
	if (!Table) return;
	mItemDataTable = Table;
}

void AItemBase::OnRep_ItemData()
{
	if (HasAuthority()) return;

	if (mItemData.Mesh)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				if (mMesh && mItemData.Mesh)
				{
					mMesh->SetStaticMesh(mItemData.Mesh);
				}
			});
	}

	if (mItemData.ItemName.IsValid())
		mItemName = mItemData.ItemName;

	if (mItemData.Tag.IsValid())
		mTag = mItemData.Tag;

	if(!mItemData.ItemRoatation.IsZero())
	mMesh->SetRelativeRotation(mItemData.ItemRoatation);

	if(!mItemData.Scale.IsZero())
	mMesh->SetRelativeScale3D(mItemData.Scale);

	mItemValue = mItemData.Value;
}


FGameplayTag  AItemBase::GetGameplayTag() const
{
	return mTag;
}
UItemDataAsset* AItemBase::GetItemDataAsset() const
{
	if (!mItemDataTable) return nullptr;

	FItemTable* Table = mItemDataTable->GetItemData(mTag);

	return Table->DataAsset;
}


FString AItemBase::GetItemName() const
{
	return mItemName.ToString();
}

