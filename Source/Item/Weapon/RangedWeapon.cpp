// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"



ARangedWeapon::ARangedWeapon()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
}


void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

