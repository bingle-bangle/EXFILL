// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeZone.h"
#include "../../Character/Player/PlayerCharacter.h"
#include "../../GameMode/ExfilGameMode.h"
#include "../../GameMode/ExfilPlayerController.h"
#include "../../GameState/ExfilPlayerState.h"
#include "../../GameState/ExfilGameState.h"
#include "PaperSprite.h"


AEscapeZone::AEscapeZone()
{
	PrimaryActorTick.bCanEverTick = true;
	mCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(mCollision);
	mCollision->SetCollisionProfileName(FName("Item"));

	mEscapeSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("EscapeSprite"));
	//mEscapeSprite->SetupAttachment(mCollision);
	//mEscapeSprite->AddLocalOffset(FVector(0, 0, 20.f));

	UPaperSprite* Sprite = Cast<UPaperSprite>(
		StaticLoadObject(UPaperSprite::StaticClass(), nullptr,
			TEXT("/Script/Paper2D.PaperSprite'/Game/UI/Item/Escape/SP_EscapeMarker.SP_EscapeMarker'"))
	);
	if (Sprite)
		mEscapeSprite->SetSprite(Sprite);



	mEscapeSprite->SetVisibility(true);
	mEscapeSprite->SetHiddenInGame(false);
	mEscapeSprite->SetVisibleInSceneCaptureOnly(true);
	mEscapeSprite->SetHiddenInSceneCapture(false);
	mEscapeSprite->SetRelativeRotation(FRotator(0, 0, -90.f));
	mEscapeSprite->SetRelativeScale3D(FVector(5.f));

	bReplicates = true;
}


void AEscapeZone::BeginPlay()
{
	Super::BeginPlay();

	mCollision->OnComponentBeginOverlap.AddDynamic(this, &AEscapeZone::OnOverlapBegin);
	mCollision->OnComponentEndOverlap.AddDynamic(this, &AEscapeZone::OnOverlapEnd);
}


void AEscapeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEscapeZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Overlap by: %s, Comp: %s"), *OtherActor->GetName(), *OtherComp->GetName()));

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (!Player)	return;

	AExfilPlayerController* PC = Cast<AExfilPlayerController>(Player->GetController());

	if (!PC) return;
	APlayerState* PlayerState = PC->PlayerState;

	if (!IsValid(PlayerState))		return;

	FString RoleString = HasAuthority() ? TEXT("Server") : TEXT("Client");

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 5.f, FColor::Green,
			FString::Printf(TEXT("[%s] BeginOverlap with %s"),
				*RoleString, *OtherActor->GetName())
		);
	}

	OnPlayerEnter(PlayerState);
}


void AEscapeZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OverlapEnd by: %s, Comp: %s"), *OtherActor->GetName(), *OtherComp->GetName()));

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (!Player)	return;

	AExfilPlayerController* PC = Cast<AExfilPlayerController>(Player->GetController());
	if (!PC) return;
	APlayerState* PlayerState = PC->PlayerState;

	if (!PlayerState)		return;


	FString RoleString = HasAuthority() ? TEXT("Server") : TEXT("Client");

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 5.f, FColor::Green,
			FString::Printf(TEXT("[%s] EndOverlap with %s"),
				*RoleString, *OtherActor->GetName())
		);
	}

	OnPlayerOut(PlayerState);
}


void AEscapeZone::CloseEscapeArea()
{
	mEscapePlayer = nullptr;
	mCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, TEXT("Player Escpae"));

	return;
}

void AEscapeZone::SetEscapeID(int32 Id)
{
	mEscapeID = Id;
}


int32 AEscapeZone::GetEscapeID() const
{
	return mEscapeID;
}

void AEscapeZone::OnPlayerEnter(APlayerState* state)
{

}
void AEscapeZone::OnPlayerOut(APlayerState* state)
{

}

void AEscapeZone::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEscapeZone, mEscapePlayer);
	DOREPLIFETIME(AEscapeZone, mEscapeID);

}