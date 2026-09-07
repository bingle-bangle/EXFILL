
#include "ExfilGameState.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../Item/Escape/EscapeZone.h"
#include "../GameMode/ExfilPlayerController.h"
#include "../GameState/ExfilPlayerState.h"

#include "../GameInstance/SessionSubsystem.h"

AExfilGameState::AExfilGameState()
{
	SetReplicates(true);

}

bool AExfilGameState::GetRadioItemState() const
{
	return bUseRadioItem;
}

void AExfilGameState::SetRadioItemState(bool State)
{
	bUseRadioItem = State;
}

void AExfilGameState::SetRadioEscapeLocation(FVector location)
{
	if (!HasAuthority()) return;

	mRadioSpawnLocations.Add(location);
	if (mRadioSpawnLocations.Num()==3)
	{
		int32 selectIndex = FMath::RandRange(0, mRadioSpawnLocations.Num() - 1);

		mRadioEscapeLocation = mRadioSpawnLocations[selectIndex];
		mSelectRadioLocation = mRadioEscapeLocation;
	}
	
}

FVector AExfilGameState::GetRadioEscapeLocation() 
{
	return mRadioEscapeLocation;
}

float AExfilGameState::GetRadioEscapeTime() const
{
	return mRadioEscpaeZoneTime;
}


bool AExfilGameState::GetFlareItemUse() const
{
	return bUseFlareItem;
}

//플레이가 끝난 플레이어의 정보를 저장
bool AExfilGameState::AddPlayer(APlayerState* PlayerState)
{
	if (!PlayerState)	return false;

	AExfilPlayerState* EscapePlayerState = Cast<AExfilPlayerState>(PlayerState);
	if (!EscapePlayerState) return false;

	FPlayerResult	PlayingInfo;
	PlayingInfo.bIsEscaped = EscapePlayerState->GetEscaped();
	PlayingInfo.KillCount = EscapePlayerState->GetKillCount();
	PlayingInfo.Killer = EscapePlayerState->GetKiller();
	//PlayingInfo.PlayerName = EscapePlayerState->GetSteamNicknameFromLocalPC(EscapePlayerState->GetPlayerController());
	PlayingInfo.PlayerName = EscapePlayerState->GetPlayerName();

	mPlayerInfoAfterEscaped.Add(PlayingInfo);
	mAlivePlayer--;

	return true;
}

//탈출 정보를 넘겨주는 함수
FString AExfilGameState::GetFinishedPlayerName()
{
	return	mPlayerInfoAfterEscaped[0].PlayerName;
}

void AExfilGameState::AddActiveEscapeZone(AActor* zone)
{
	if (!zone) return;

	AEscapeZone* EZ = Cast<AEscapeZone>(zone);
	if (!EZ) return;

	FEscapeInfo  Info;
	Info.EscapeID = EZ->GetEscapeID();
	Info.Zone = EZ;

	mActiveEscapeZones.Add(EZ->GetEscapeID(), EZ);
	mActiveEscapeZoneForClient.Add(Info);

}

bool AExfilGameState::CloseEscapeZone(int32 EscapeId)
{
	AEscapeZone** TempZone = mActiveEscapeZones.Find(EscapeId);
	if (!TempZone) return false;

	AEscapeZone* CloseZone = *TempZone;
	CloseZone->CloseEscapeArea();
	CloseZone->Destroy();
	CloseZone = nullptr;
	TempZone = nullptr;
	mActiveEscapeZones.Remove(EscapeId);
	mActiveEscapeZoneForClient[EscapeId];

	return true;
}

void AExfilGameState::OnRep_RadioLocation()
{
	mRadioEscapeLocation = mSelectRadioLocation;
}

void AExfilGameState::BeginPlay()
{
	Super::BeginPlay();

	USessionSubsystem* Session = GetGameInstance()->GetSubsystem<USessionSubsystem>();

	if (Session)
	{
		mAllPlayer = Session->GetPlayerCount();
		mAlivePlayer = mAllPlayer;
	
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, FString::Printf(TEXT("ExfilGameState::mAllPlayer : %d"), mAllPlayer));
	}

	//@ToDo : 에디터 테스트 용. 삭제 예정
	if (mAllPlayer == 0)
	{
		mAllPlayer = 2;
		mAlivePlayer = mAllPlayer;
	}
}

void AExfilGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExfilGameState, bUseRadioItem);
	DOREPLIFETIME(AExfilGameState, bFindEscapeZone);
	DOREPLIFETIME(AExfilGameState, mRadioEscpaeZoneTime);
	DOREPLIFETIME(AExfilGameState, mAllPlayer);
	DOREPLIFETIME(AExfilGameState, mAlivePlayer);
	DOREPLIFETIME(AExfilGameState, mSuccessEscapePlayerCount);
	DOREPLIFETIME(AExfilGameState, mPlayerInfoAfterEscaped);
	DOREPLIFETIME(AExfilGameState, mActiveEscapeZoneForClient);
	DOREPLIFETIME(AExfilGameState, mSelectRadioLocation);
}

// 모든 PlayerStart 배열에 저장
void AExfilGameState::SetPlayerStarts(TArray<AActor*> PlayerStarts)
{
	mPlayerStarts = PlayerStarts;

	// 랜덤으로 스폰하기 위해 배열 섞기
	FMath::RandInit(FDateTime::Now().GetMillisecond());

	int Num = mPlayerStarts.Num();

	for (int32 i = Num - 1; i > 0; i--)
	{
		int32 Idx = FMath::RandRange(0, i);

		if(i != Idx)
			mPlayerStarts.Swap(i, Idx);
	}

	int32 Result = mPlayerStarts.Num();
}