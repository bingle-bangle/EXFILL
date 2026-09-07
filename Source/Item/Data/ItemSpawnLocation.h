// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnLocation.generated.h"

UCLASS()
class EXFIL_API AItemSpawnLocation : public AActor
{
	GENERATED_BODY()
	
public:
	AItemSpawnLocation();

	UPROPERTY(EditAnywhere)
	ESpawnType	mType = ESpawnType::None;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* mRoot;
	
#if WITH_EDITORONLY_DATA
	/** Editor에서만 보이는 아이콘 */
	UPROPERTY()
	UBillboardComponent* EditorBillboard;
#endif

protected:
	virtual void BeginPlay();

};
