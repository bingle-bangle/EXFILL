// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioEscapeZone.h"
#include "../../GameMode/ExfilPlayerController.h"

ARadioEscapeZone::ARadioEscapeZone()
{
	//Mesh 체인지
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mMesh"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/BoatPack/Meshes/RubberBoat/SM_RubberBoatWhole.SM_RubberBoatWhole'"));
	if (Mesh.Succeeded())
		mMesh->SetStaticMesh(Mesh.Object);

	mMesh->SetupAttachment(mCollision);
	mMesh->SetRelativeLocation(FVector(-50, 0, -40));
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Collision 크기 체인지
	mCollision->SetSphereRadius(150.f);
	
	//mStandbyTime = 60.f;
	mStandbyTime = 10.f;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Script/Engine.Material'/Game/UI/Item/Escape/MT_EscapeMarker.MT_EscapeMarker'"));
	if (MaterialAsset.Succeeded())
		mEscapeSprite->SetMaterial(0, MaterialAsset.Object);

	mEscapeSprite->SetupAttachment(mMesh);
	mEscapeSprite->SetRelativeLocation(FVector(50.f, 0, 220.f));

	bReplicates = true;
	bAlwaysRelevant = true;
	
}


void ARadioEscapeZone::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("RadioEscapeZone beginplay"));
}

void ARadioEscapeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mAlivetime += DeltaTime;
	//GEngine->AddOnScreenDebugMessage(1, 50.f, FColor::Yellow, FString::Printf(TEXT("RadioEscapeZone Alive : %f"), mAlivetime));
	//GEngine->AddOnScreenDebugMessage(2, 50.f, FColor::Blue, FString::Printf(TEXT("RadioEscapeZone Location : %s"), *GetActorLocation().ToString()));

}
void ARadioEscapeZone::OnPlayerEnter(APlayerState* state)
{
	Super::OnPlayerEnter(state);

	if (!state) { GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, TEXT("RadioEscapeZone Enter No State")); return; }

	AExfilPlayerController* EPC = Cast<AExfilPlayerController>(state->GetOwningController());
	if (!EPC) return;

	if (EPC->IsLocalController())
	{
		
		EPC->EscapeProgressbarSet(5);
	}
}

void ARadioEscapeZone::OnPlayerOut(APlayerState* state)
{
	Super::OnPlayerEnter(state);

	if (!state) { GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, TEXT("RadioEscapeZone Enter No State")); return; }


	AExfilPlayerController* EPC = Cast<AExfilPlayerController>(state->GetOwningController());
	if (!EPC) return;

	if (EPC->IsLocalController())
	{
		EPC->CancleProgressbar();
	}

}


