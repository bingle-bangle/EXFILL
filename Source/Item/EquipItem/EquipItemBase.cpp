// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemBase.h"
#include "../ItemDataAsset.h"

#include "../../GameInstance/ItemDataSubsystem.h"

// Sets default values
AEquipItemBase::AEquipItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mMesh);

	bReplicates = true;
	SetReplicateMovement(true);
	mMesh->SetIsReplicated(true);
}


void AEquipItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipItemBase::OnRep_ChangeMesh()
{
	mMesh->SetSkeletalMesh(mMeshAsset);
}

//인벤토리에서 장비 아이템 소환시
//초기 세팅을 기본으로 해주는 함수
void AEquipItemBase::SetItemSetting(FGameplayTag ItemTag)
{
	if (!ItemTag.IsValid()) return;

	FDataForWeapon* ItemData = UItemDataSubsystem::GetInstance(GetWorld())->GetWeaponData(ItemTag);

	if (!ItemData)
		return;

	mTag = ItemTag;

	mMesh->SetSkeletalMesh(ItemData->SKM);
	mSocketHand = ItemData->HandSocket;
	mSocketHolster = ItemData->WaistSocket;
	mAttackValue = ItemData->Value;

	// 클라이언트 Replicate 하기 위함
	mMeshAsset = ItemData->SKM;
}

void AEquipItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEquipItemBase, mMeshAsset);
}
