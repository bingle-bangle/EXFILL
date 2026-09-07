// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnLocation.h"



AItemSpawnLocation::AItemSpawnLocation()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

#if WITH_EDITOR
	// 에디터 전용 표시용 Billboard
	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
	EditorBillboard->SetupAttachment(mRoot);
	EditorBillboard->SetHiddenInGame(true);  // 게임 중엔 안 보이게
#endif
}

void AItemSpawnLocation::BeginPlay()
{
	Super::BeginPlay();
}