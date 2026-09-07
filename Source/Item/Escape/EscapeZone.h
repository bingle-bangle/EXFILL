// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "EscapeZone.generated.h"



UCLASS()
class EXFIL_API AEscapeZone : public AActor
{
	GENERATED_BODY()

public:

	AEscapeZone();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;


	virtual void	CloseEscapeArea();
	void	SetEscapeID(int32 Id);
	int32   GetEscapeID() const;


protected:

	virtual void BeginPlay() override;

	virtual void OnPlayerEnter(APlayerState* state);
	virtual void OnPlayerOut(APlayerState* state);
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult
	);
	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent>	mCollision;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> mEscapeSprite;
	
	UPROPERTY(EditAnywhere, Category = "EscapeTime")
	float	mStandbyTime=0;

	UPROPERTY(Replicated)
	APlayerState* mEscapePlayer;
	UPROPERTY(Replicated)
	int32	mEscapeID = 0;

	FTimerHandle	mCloseTimer;


};
