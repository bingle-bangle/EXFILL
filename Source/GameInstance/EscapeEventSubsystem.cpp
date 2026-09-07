// Fill out your copyright notice in the Description page of Project Settings.



#include "EscapeEventSubsystem.h"
#include "../GameMode/ExfilGameMode.h"
#include "../GameState/ExfilPlayerState.h"
#include "../GameMode/ExfilPlayerController.h"
#include "../GameInstance/SpawnManager.h"
#include "../GameState/ExfilGameState.h"
#include "../Item/Escape/FlareEscapeZone.h"
#include "../Item/Escape/RadioEscapeZone.h"


UEscapeEventSubsystem::UEscapeEventSubsystem()
{

}

UEscapeEventSubsystem* UEscapeEventSubsystem::GetEventSubsystem(const UObject* World)
{
	if (!IsValid(World)) return nullptr;
	return UGameplayStatics::GetGameInstance(World)->GetSubsystem<UEscapeEventSubsystem>();
}

void UEscapeEventSubsystem::StartRadioEvent(AController* con)
{
	AExfilPlayerController* EPC = Cast<AExfilPlayerController>(con);
	if (!EPC) { UE_LOG(LogTemp, Warning, TEXT("No PlayerController")); return; }

	AExfilGameMode* EGM = GetWorld()->GetAuthGameMode<AExfilGameMode>();

	AExfilPlayerState* EPS = EPC->GetPlayerState<AExfilPlayerState>();
	if (!EPS) return;

	if (!EGM || !con)  return; 

	if (EGM->CheckEscapeItem(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Escape.Radio")))))
	{
		
		RequestEscapeZoneEnable(FString(TEXT("Radio")), FVector(0,0,0));
		EscapeUI(EPC, TEXT("Radio"));
	}
	else
	{
		EPC->PopUpAnouncmentWidget(EUIState::Standby, FString(TEXT("Radio")));
	}

}

void UEscapeEventSubsystem::StartFlareEvent(AController* con, FVector userlocation)
{
	AExfilGameMode* EGM = GetWorld()->GetAuthGameMode<AExfilGameMode>();
	AExfilPlayerController* EPC = Cast<AExfilPlayerController>(con);
	if (!EGM || !con) { GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Orange, FString(TEXT("No Controller or No GameMode"))); return; }

	AExfilPlayerState* EPS = EPC->GetPlayerState<AExfilPlayerState>();
	if (!EPS) return;

	if (EGM->CheckEscapeItem(FGameplayTag::RequestGameplayTag(FName(TEXT("Item.Escape.Flare")))))
	{

		UWorld* Sw = EGM->GetWorld();
		RequestEscapeZoneEnable(FString(TEXT("Flare")), EPC->GetPawn()->GetActorLocation());
		//USpawnManager::GetSpawnManager(GetWorld())->SpawnEnemyForEscapeEvent(5, EPC);
		EscapeUI(EPC, TEXT("Flare"));
	}
	else
	{
		EPC->PopUpAnouncmentWidget(EUIState::Standby, FString(TEXT("Flare")));
	}
}


//탈출 지역 활성화 및 시간초 세팅
void UEscapeEventSubsystem::RequestEscapeZoneEnable(FString escapename, FVector location)
{

	AExfilGameState* EGS = GetWorld()->GetGameState<AExfilGameState>();
	if (!EGS) return;

	AActor* Zone = nullptr;

	//탈출지역 스포너에게 스폰 지시
	if (escapename == FString(TEXT("Radio")))
	{
		//게임스테이트에서 위치받아와서 소환
		Zone = USpawnManager::GetSpawnManager(GetWorld())->SpawnRadioEscapeZone(EGS->GetRadioEscapeLocation());
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Yellow, FString::Printf(TEXT("Radio spwan location : %s"), *EGS->GetRadioEscapeLocation().ToString()));
		ARadioEscapeZone* REZ = Cast<ARadioEscapeZone>(Zone);

		//타이머 콜백으로 바꿀 것
		/*mRadioDel.BindUFunction(EGS, FName("CloseEscapeZone"), REZ->GetEscapeID());
		GetWorld()->GetTimerManager().SetTimer(mEscapeZoneActive, mRadioDel, mRadioZoneTime, false);*/
	}
	else if (escapename == FString(TEXT("Flare")))
	{

		Zone = USpawnManager::GetSpawnManager(GetWorld())->SpawnFlareEscapeZone(location);
		AFlareEscapeZone* FZ = Cast<AFlareEscapeZone>(Zone);
		FZ->SetHelicoptor(location);
		FZ->StartHelicopterEvent();
	}
	if (Zone)
	{
		//소환에 성공하면 스테이트에 저장
		EGS->AddActiveEscapeZone(Zone);
	}
}

//아이템 사용 여부에 따라 Ui를 다르게 실행하는 함수
void UEscapeEventSubsystem::EscapeUI(AExfilPlayerController* epc, FString itemname)
{
	if (!epc || itemname.IsEmpty()) return;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (auto PC = Cast<AExfilPlayerController>(It->Get()))
		{
			if (PC == epc)
			{
				PC->PopUpAnouncmentWidget(EUIState::User, itemname);
			}
			else
			{
				PC->PopUpAnouncmentWidget(EUIState::Other, itemname);
			}
		}
	}
}





