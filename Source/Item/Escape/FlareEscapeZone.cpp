// Fill out your copyright notice in the Description page of Project Settings.


#include "FlareEscapeZone.h"
#include "../../GameMode/ExfilPlayerController.h"
#include "../../Character/Player/PlayerCharacter.h"
#include "../../Component/InteractComponent.h"
#include "NiagaraComponent.h"
#include "FlareAnimInstance.h"
#include "../../UI/Exfil/HUDWidget.h"

/*
TODO : 오버랩 범위 넓혀야함

*/

AFlareEscapeZone::AFlareEscapeZone()
{
	PrimaryActorTick.bCanEverTick = true;
	//Collision 크기 체인지

	mCollision->SetSphereRadius(300.f);

	mStandbyTime = 5.f;

	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("mMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SM(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/West_Heli_UH60A/SK_West_Heli_UH60A.SK_West_Heli_UH60A'"));
	if (SM.Succeeded())
		mMesh->SetSkeletalMesh(SM.Object);

	mMesh->SetupAttachment(mCollision);
	mMesh->SetRelativeLocation(FVector(10, 0, 240.f));
	mEscapeSprite->SetupAttachment(mMesh);

	mDustPlume = CreateDefaultSubobject<UNiagaraComponent>(TEXT("mDustPlume"));
	mLeftExhaust = CreateDefaultSubobject<UNiagaraComponent>(TEXT("mLeftExhaust"));
	mRightExhaust = CreateDefaultSubobject<UNiagaraComponent>(TEXT("mRightExhaust"));

	mDustPlume->SetupAttachment(mCollision);
	mLeftExhaust->SetupAttachment(mCollision);
	mRightExhaust->SetupAttachment(mCollision);

	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NSD(TEXT("/Script/Niagara.NiagaraSystem'/Game/VigilanteContent/Shared/Particles/ParticleSystems/NS_Heli_Ground_Dust.NS_Heli_Ground_Dust'"));
	//if (NSD.Succeeded()) { mDustPlume->SetAsset(NSD.Object); mDustPlume->bAutoActivate = false; };

	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NSLF(TEXT("/Script/Niagara.NiagaraSystem'/Game/Assets/West_Heli_UH60A/FX/NS_Heli_Ground_Dust.NS_Heli_Ground_Dust'"));
	//if (NSLF.Succeeded()) 
	//{ 
	//	mLeftExhaust->SetAsset(NSLF.Object);  
	//	mRightExhaust->SetAsset(NSLF.Object);

	//	mLeftExhaust->bAutoActivate = false;
	//	mRightExhaust->bAutoActivate = false;
	//}

	static ConstructorHelpers::FObjectFinder<USoundBase> SB(TEXT("/Script/Engine.SoundWave'/Game/Assets/West_Heli_UH60A/HelicopterSound.HelicopterSound'"));
	if (SB.Succeeded())
		mEngineSound = SB.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Script/Engine.Material'/Game/UI/Item/Escape/MT_FlareMarker.MT_FlareMarker'"));
	if (MaterialAsset.Succeeded())
		mEscapeSprite->SetMaterial(0, MaterialAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Assets/West_Heli_UH60A/ABP_FlareAnimInstance.ABP_FlareAnimInstance_C'"));
	if (Anim.Succeeded())
		mMesh->SetAnimInstanceClass(Anim.Class);

	mEscapeSprite->SetRelativeLocation(FVector(0, 0, 500.f));

	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
	
}


void AFlareEscapeZone::BeginPlay()
{
	Super::BeginPlay();
	
	mHeliCopAnim = Cast<UFlareAnimInstance>(mMesh->GetAnimInstance());

	
}

void AFlareEscapeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActive)
	{
		if (!mHeliCopAnim) return;
		
		mHeliCopAnim->mMainRotorSpeed.Yaw += 10;
		mHeliCopAnim->mTailRotorSpeed.Roll += 10;
	}

}

void  AFlareEscapeZone::SetHelicoptor(FVector location)
{
	if (!HasAuthority()) return;
	location.Z += 30.f;
	mTargetLocation = location;
	mStartLocation = location + FVector(-30000.f, 0.f, 0.f);
	mStartLocation.Z = 10000.f;


}
//탈출 이벤트 트리거
void AFlareEscapeZone::StartHelicopterEvent()
{
	
	if (HasAuthority())
	{
		HelicopterEvent();
	}
	else
	{
		HelicopterEvent();
	}
}

void AFlareEscapeZone::HelicopterEvent_Implementation()
{
	//소환 타이머 세팅
	//GetWorld()->GetTimerManager().SetTimer(mHiddenTime, this, &AFlareEscapeZone::FlyingStart, mActivateTime, false);
	FlyingStart();
}

//탈출 상호작용 함수
void AFlareEscapeZone::OnPlayerEnter(APlayerState* state)
{
	Super::OnPlayerEnter(state);

	if (!state) { GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, TEXT("RadioEscapeZone Enter No State")); return; }

	AExfilPlayerController* EPC = Cast<AExfilPlayerController>(state->GetOwningController());
	if (!EPC) return;

	if (EPC->IsLocalController())
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(state->GetPawn());
		if (!IsValid(Player)) return;

		UInteractComponent* Interact = Player->GetInteractComponent();
		if (!IsValid(Interact)) return;

		EPC->GetHUDWidget()->SetProgressbarState(true);

		Interact->SetEscapeState(true);
		EPC->SetEscapeId(mEscapeID);
		Player->SetEscapedState(true);
	
	}

}

void AFlareEscapeZone::OnPlayerOut(APlayerState* state)
{
	Super::OnPlayerEnter(state);

	if (!state) { GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, TEXT("RadioEscapeZone Enter No State")); return; }

	AExfilPlayerController* EPC = Cast<AExfilPlayerController>(state->GetOwningController());
	if (!EPC) return;

	if (EPC->IsLocalController())
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(state->GetPawn());
		if (!IsValid(Player)) return;

		UInteractComponent* Interact = Player->GetInteractComponent();
		if (!IsValid(Interact)) return;

		EPC->GetHUDWidget()->SetProgressbarState(false);
		EPC->SetEscapeId(-1);
		Interact->SetEscapeState(false);
		Player->SetEscapedState(false);
	
	}
}



//헬리콥터 이동 연출
void AFlareEscapeZone::UpdateHelicopterLocation()
{
	if (!HasAuthority()) return;

	//헬기가 얼마나 움직였는지 저장
	mElapsedTime += GetWorld()->GetDeltaSeconds();

	//전체 이동거리 대미 현재까지 진행률을 정규화
	float Alpha = FMath::Clamp(mElapsedTime / mApproachTime, 0.f, 1.f);


	//일정한 속도로 이동시키기위한 선형보간
	FVector MoveLocation=FVector();
	MoveLocation.X = FMath::Lerp(mStartLocation.X, mTargetLocation.X, Alpha);
	MoveLocation.Y = FMath::Lerp(mStartLocation.Y, mTargetLocation.Y, Alpha);
	MoveLocation.Z = 10000.f;


	if (mHelicopterAudio)
	{
		float Distance = FVector::Dist(MoveLocation, mTargetLocation);
		float Volume = FMath::GetMappedRangeValueClamped(FVector2D(4000.f, 500.f), FVector2D(0.2f, 1.0f), Distance);
		mHelicopterAudio->SetVolumeMultiplier(Volume);
		float Pitch = FMath::Lerp(1.f, 0.8f, Alpha);
		mHelicopterAudio->SetPitchMultiplier(Pitch);
	}

	if (Alpha < 0.8f)
	{
		// 아직 접근 중이면 Z 고정
		MoveLocation.Z = mStartLocation.Z;
	}
	else
	{
		// 0.8~1.0 구간을 0~1로 재정규화
		float LandingAlpha = (Alpha - 0.8f) / 0.2f; // 0.8 → 0, 1.0 → 1
		float DesiredHoverHeight = 50.f;
		MoveLocation.Z = FMath::Lerp(mStartLocation.Z, mTargetLocation.Z, LandingAlpha);


		float CurrentHeight = MoveLocation.Z;
		float TargetHeight = mTargetLocation.Z + DesiredHoverHeight; // 예: 지면 위 200cm 유지

		// 목표 고도 이하로 내려갔으면 착륙 완료 처리
		if (CurrentHeight <= TargetHeight + 5.f) // 오차 허용치 5cm
		{
			MoveLocation.Z = TargetHeight;

			// 타이머 클리어 (이동 멈춤)
			GetWorld()->GetTimerManager().ClearTimer(mFlyTime);

			// 착륙 이벤트 호출
			HelicoterLanded();
		}
	}

	SetActorLocation(MoveLocation);
}
//이동중 모션
void AFlareEscapeZone::Client_Flying_Implementation()
{

	FVector SpawnLocation = mTargetLocation + mHelicopterOffset;

	if (mEngineSound)
	{
		//멀리서 소리가 작게나게 설정
		mHelicopterAudio = UGameplayStatics::SpawnSoundAtLocation(this, mEngineSound, SpawnLocation, FRotator::ZeroRotator, 0.2f);
	}
}

//헬리콥터 착지 
void AFlareEscapeZone::HelicoterLanded()
{

	if (!HasAuthority()) return;
	GetWorld()->GetTimerManager().ClearTimer(mFlyTime);

	bActiveState = false;

	Client_Landing();
}

void AFlareEscapeZone::Client_Landing_Implementation()
{

	//엔진음 피치 낮추기 + 착륙 사운드 실행
	if (mHelicopterAudio)
	{
		//mHelicopterAudio->SetPitchMultiplier(0.6f);
	}

	if (mLandingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, mLandingSound, mTargetLocation);
	}


}

void AFlareEscapeZone::OnRep_HelicopterVisible()
{
	SetActorHiddenInGame(!bHelicopterVisible);
}

void AFlareEscapeZone::OnRep_ActiveState()
{
	bActive = bActiveState;
}

void AFlareEscapeZone::FlyingStart()
{
	if (!HasAuthority()) return;

	bHelicopterVisible = true;

	bActive = true;
	bActiveState = true;

	mElapsedTime = 0.f;
	GetWorld()->GetTimerManager().SetTimer(mFlyTime, this, &AFlareEscapeZone::UpdateHelicopterLocation, 0.02f, true);
	Client_Flying();
}





void AFlareEscapeZone::SetDustPlumeIntensity(float intensity)
{
	if (!mDustPlume) return;

	float Intensity = intensity * 200.f;

	mDustPlume->SetNiagaraVariableFloat(TEXT(""), Intensity);

}


void AFlareEscapeZone::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlareEscapeZone, bHelicopterVisible);
	DOREPLIFETIME(AFlareEscapeZone, bActiveState);
	DOREPLIFETIME(AFlareEscapeZone, bCurrentVolume);

}


void AFlareEscapeZone::CloseEscapeArea()
{

	Super::CloseEscapeArea();

	if (mHelicopterAudio->IsActive())
	{
		mHelicopterAudio->SetActive(false);
		bCurrentVolume = false;
	}

	return;
}


void AFlareEscapeZone::OnRep_HelicopterVolume()
{
	mHelicopterAudio->SetActive(bCurrentVolume);
}