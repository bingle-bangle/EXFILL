// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "EscapeZone.h"
#include "FlareEscapeZone.generated.h"


class UFlareAnimInstance;
UCLASS()
class EXFIL_API AFlareEscapeZone : public AEscapeZone
{
	GENERATED_BODY()
	
	
public:
	AFlareEscapeZone();

	void	SetHelicoptor(FVector location);

	void	StartHelicopterEvent();

	UFUNCTION(Server, Reliable)
	void	HelicopterEvent();
	void	HelicopterEvent_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;

	virtual void Tick(float DeltaTime);

protected:
	virtual void BeginPlay() override;

	virtual void OnPlayerEnter(APlayerState* state);
	virtual void OnPlayerOut(APlayerState* state);

	void UpdateHelicopterLocation();
	void HelicoterLanded();
	
	virtual void	CloseEscapeArea();

	void SetDustPlumeIntensity(float intensity);
	void SetMainRotorSpeed(float mainrotorspeed);
	void SetTailRotorSpeed(float tailrotorspeed);
	void SetFronWheels(float frontwheelsstate);
	void SetRearWheels(float rearwheelsstate);
	void SetRearDoors(float left, float right);

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent>	mDustPlume;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent>	mLeftExhaust;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent>	mRightExhaust;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool	bIsDuastPlumeVisible = false;

private:

	UFUNCTION(NetMulticast, Reliable)
	void	Client_Flying();
	void	Client_Flying_Implementation();

	UFUNCTION(NetMulticast,Reliable)
	void	Client_Landing();
	void	Client_Landing_Implementation();


	UFUNCTION()
	void OnRep_HelicopterVisible();

	UPROPERTY(ReplicatedUsing = OnRep_HelicopterVisible)
	bool	bHelicopterVisible = true;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveState)
	bool	bActiveState = false;

	UFUNCTION()
	void OnRep_ActiveState();

	bool bActive = false;
	struct FRotorSpeed
	{
		float speed;
	};

	FRotorSpeed   mRotorSpeed;
	

	void FlyingStart();

	

	FVector		mHelicopterOffset;
	float		mApproachTime = 50.f;
	float		mActivateTime = 0.f;

	USoundBase*      mEngineSound;
	USoundBase*      mLandingSound;
	UAudioComponent* mHelicopterAudio;

	UPROPERTY(ReplicatedUsing = OnRep_HelicopterVolume)
	bool	bCurrentVolume = false;

	UFUNCTION()
	void  OnRep_HelicopterVolume();

	FVector		mStartLocation;
	FVector		mTargetLocation;
	float		mElapsedTime = 0.f;

	FTimerHandle	mHiddenTime;	
	FTimerHandle    mFlyTime;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent>	mMesh;

	TObjectPtr<UFlareAnimInstance>	mHeliCopAnim;


	
};
