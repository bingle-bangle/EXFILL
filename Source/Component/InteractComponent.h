
/*
	생성 날짜 : 2025.09.02

	생성자 : 김서정

	기능 :
		- 오브젝트와 상호작용 처리
		-
*/
/////////////////////////
#pragma once

#include "../GameInfo.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"



class AItemBase;
class UItemDataAsset;
class AEquipItemBase;

class AFlareItem;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXFIL_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractComponent();

private:
	class APlayerCharacter* mOwner = nullptr;

	TObjectPtr<AItemBase>	mInteractItem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Item")
	float	mTraceRadius = 100.f;
	TObjectPtr<AEquipItemBase>	mEquipItem;
	FTimerHandle     mInteractTimer;

	bool	bEscape = false;

	UFUNCTION(Server,Reliable)
	void EscapeEventTrigger(const FString& text);
	void EscapeEventTrigger_Implementation(const FString& text);

public:
	void SetOwnerCharacter(APlayerCharacter* Owner)
	{
		mOwner = Owner;
	}

	void SetEscapeState(bool state)
	{
		bEscape = state;
	}

public:
	void SetInteractItem();
	void InteractTraceSetting();

	void InputInteract();

	UFUNCTION(Server, Reliable)
	void DeleteItem(AItemBase* Item);
	void DeleteItem_Implementation(AItemBase* Item);

	UFUNCTION()
	void EscapeEventDelegate(FString text);
};