// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameState.h"
#include "ExfilGameState.generated.h"

class APlayerCharacter;
class AEscapeZone;

USTRUCT(BlueprintType)
struct FEscapeInfo
{
	GENERATED_BODY()

public:
	int32  EscapeID;
	AEscapeZone* Zone;
	bool   bRadioEscapeZone = false;
};


USTRUCT(BlueprintType)
struct FPlayerResult
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString PlayerName;
	UPROPERTY()
	FString	Killer;
	UPROPERTY()
	int32	KillCount = 0;
	UPROPERTY()
	bool	bIsEscaped = false;
};

UCLASS()
class EXFIL_API AExfilGameState : public AGameState
{
	GENERATED_BODY()

public:
	AExfilGameState();

	bool GetRadioItemState() const;
	void SetRadioItemState(bool State);
	void    SetRadioEscapeLocation(FVector location);
	FVector GetRadioEscapeLocation();
	float	GetRadioEscapeTime() const;


	bool GetFlareItemUse() const;


	void	GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;

	bool	AddPlayer(APlayerState* PlayerState);

	FString	 GetFinishedPlayerName();


	void AddActiveEscapeZone(AActor* zone);

	UFUNCTION()
	bool CloseEscapeZone(int32 EscapezoneNumber);

	FString GetName(int32 index) const
	{
		return mPlayerInfoAfterEscaped[index].PlayerName;
	}


private:
	//무전기 아이템 사용여부 체크
	UPROPERTY(Replicated)
		bool	bUseRadioItem = false;
	UPROPERTY(Replicated)
		bool	bUseFlareItem = false;
	//탈출지역 상태 관리
	UPROPERTY(Replicated)
		bool	bFindEscapeZone = false;
	UPROPERTY(Replicated)
		FVector mRadioEscapeLocation;
	UPROPERTY(Replicated)
		float	mRadioEscpaeZoneTime = 300.f;

	//세션에 참가한 총 플레이어의 수
	UPROPERTY(Replicated)
		int32	mAllPlayer = 10;

	// 남아 있는 플레이어의 수
	UPROPERTY(Replicated)
		int32	mAlivePlayer = 10;

	//탈출에 성공한 플레이어의 수
	UPROPERTY(Replicated)
		int32	mSuccessEscapePlayerCount = 0;


	//게임이 끝난 플레이어정보 저장
	//UPROPERTY(Replicated)
	UPROPERTY(Replicated)
	TArray<FPlayerResult>	mPlayerInfoAfterEscaped;

	UPROPERTY(VisibleAnywhere)
		float mRoundTime = 600.f;

	UPROPERTY(Replicated)
	TArray<FEscapeInfo>   mActiveEscapeZoneForClient;

	TMap<int32, AEscapeZone*>	mActiveEscapeZones;

	TArray<FVector>	mRadioSpawnLocations;
	
	UPROPERTY(ReplicatedUsing = OnRep_RadioLocation)
	FVector		mSelectRadioLocation;

	UFUNCTION()
	void OnRep_RadioLocation();

protected:
	virtual void BeginPlay() override;

public:
	float  GetRoundTime() const
	{
		return mRoundTime;
	}

	int32 GetAlivePlayer()
	{
		return mAlivePlayer;
	}

	TArray<FPlayerResult> GetPlayerResultList()
	{
		return mPlayerInfoAfterEscaped;
	}

// ===== Player Start =====
protected:
	TArray<AActor*> mPlayerStarts;

public:
	// 모든 PlayerStart 배열에 저장
	void SetPlayerStarts(TArray<AActor*> PlayerStarts);

	// 해당 인덱스의 PlayerStart 반환
	AActor* GetPlayerStart(int32 Index)
	{
		return mPlayerStarts[Index % mPlayerStarts.Num()];
	}
};

