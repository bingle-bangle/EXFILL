
#include "InteractComponent.h"
#include "Inventory/InventoryComponent.h"
#include "../Item/ItemBase.h"
#include "../Item/ItemDataAsset.h"
#include "../Character/Player/PlayerCharacter.h"
#include "../GameMode/ExfilPlayerController.h"
#include "../UI/Item/ItemPickUpWidget.h"
#include "../GameInstance/SpawnManager.h"
#include "../GameInstance/EscapeEventSubsystem.h"
#include "../UI/Exfil/HUDWidget.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);
}


/*
	작성자:	김서정
	날짜:	2025.09.08
	기능:
		- 상호작용 키 입력 받으면 실행 (Client)
		- SetInteractItem에 의해 결정된 mInteractItem과 상호작용
		- 액터의 GameplayTag에 따라 분기
*/
void UInteractComponent::InputInteract()
{

	if (bEscape)
	{
		return;
	}

	if (!mInteractItem)
		return;

	// add : 김다해, 보급품이면 줍지 말고 루팅 열기
	if (ASupplyItem* Supply = Cast<ASupplyItem>(mInteractItem))
	{
		if (AExfilPlayerController* PC = Cast<AExfilPlayerController>(mOwner ? mOwner->GetController() : nullptr))
		{
			if (PC->IsInventoryVisible())
			{
				PC->ToggleInventory();
			}
			PC->Server_BeginLoot(Supply);
		}
		return;
	}
	else
	{
		FGameplayTag Tag = mInteractItem->GetGameplayTag();

		if (!Tag.IsValid())
			return;

		// 상호작용 종류에 따라 다른 함수 실행


		// 아이템 획득
		UItemDataAsset* ItemAsset = mInteractItem->GetItemDataAsset();

		if (!ItemAsset)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("No DataAsset"));
			return;
		}

		if (mOwner && mOwner->GetInventoryComponent())
		{
			//const bool bStored = mOwner->GetInventoryComponent()->RequestPickupByData(ItemAsset);
			const int32 CountToPickup = mInteractItem->GetStackCount();
			const bool bStored = mOwner->GetInventoryComponent()->RequestPickupByData(ItemAsset, CountToPickup);

			if (bStored)
			{
				mInteractItem->SwitchView(false);

				DeleteItem(mInteractItem);

				mInteractItem = nullptr;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Inventory full or placement failed"));

			}
		}
	}

}

void UInteractComponent::DeleteItem_Implementation(AItemBase* Item)
{
	if (Item)
		Item->Destroy();
}



/*

  작성자 : 조소정
  기  능 : EscapeEventTrigger
           InteractTraceSetting
		   EscapeEventDelegate

*/
//탈출 이벤트 트리거 함수
void UInteractComponent::EscapeEventDelegate(FString text)
{
	EscapeEventTrigger(text);
}

void UInteractComponent::EscapeEventTrigger_Implementation(const FString& text)
{
	if (text.IsEmpty()) return;

	if (text == TEXT("Item.Escape.Flare"))
	{
		UEscapeEventSubsystem::GetEventSubsystem(GetWorld())->StartFlareEvent(mOwner->GetController(), mOwner->GetActorLocation());
	}
	else if (text == TEXT("Item.Escape.Radio"))
	{
		UEscapeEventSubsystem::GetEventSubsystem(GetWorld())->StartRadioEvent(mOwner->GetController());
	}
}

//아이템과 오버랩됐을시 가장 가까운 한개의 아이템이 무엇인지 계산해주는 함수
void UInteractComponent::SetInteractItem()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

	if (Player == nullptr)
	{
		mInteractItem = nullptr;
		return;
	}

	AExfilPlayerController* OwningController = Cast<AExfilPlayerController>(Player->GetController());
	if (!OwningController || !OwningController->GetHUDWidget()) return;

	FRotator Origin = OwningController->GetControlRotation();
	FVector Lot = (Player->GetActorLocation()+ FVector(15.f,0,-30.f)) + Origin.Vector() * 100.f;
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Lot, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6, FCollisionShape::MakeSphere(mTraceRadius));


	if (mInteractItem)
	{
		mInteractItem->SwitchView(false);
	}

	mInteractItem = nullptr;
	float HighestWeight = -3.5;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		if (AActor* HitActor = Overlap.GetActor())
		{
			if (AItemBase* Item = Cast<AItemBase>(HitActor))
			{
				FVector Dir = (HitActor->GetActorLocation() - Lot).GetUnsafeNormal();

				float Result = FVector::DotProduct(Dir, mOwner->GetControlRotation().Vector());
				float Weight = Result * 10.f;

				if (HighestWeight < Weight)
				{
					mInteractItem = Item;
					HighestWeight = Weight;
				}
			}
		}
	}
	if (mInteractItem)
	{
		
		OwningController->GetHUDWidget()->SetItemNameInTextWidget(mInteractItem->GetItemName());
		OwningController->GetHUDWidget()->SetPickUpWidgetVisibility(true);
		mInteractItem->SwitchView(true);
	}
	else
	{
		OwningController->GetHUDWidget()->SetPickUpWidgetVisibility(false);
	}

}

//상호작용 트레이스 타이머 설정 함수
void UInteractComponent::InteractTraceSetting()
{
	if (!mOwner) return;

	if (mOwner->IsLocallyControlled())
		GetWorld()->GetTimerManager().SetTimer(mInteractTimer, this, &ThisClass::SetInteractItem, 0.5f, true);
}

