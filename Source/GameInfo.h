
#pragma once

#include "EngineMinimal.h"
#include "Engine.h"
#include "Engine/OverlapResult.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

#include "Kismet/KismetmathLibrary.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayCueNotify_Static.h"

#include "Networking.h"
#include "Net/UnrealNetwork.h"

#include "NiagaraSystem.h"
#include "PaperSpriteComponent.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/VoiceInterface.h"

#include "GameInfo.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define Aiming_Trace  ECC_GameTraceChannel5

#define		TeamPlayer		10
#define		TeamEnemy		30
#define		TeamNeutral		50

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,
	Crouch,
	Jump,
	Fall,
	Hit,
	Dead
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Find,
	Attack,
	Wait,
	Dead
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Melee,
	Ranged
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Hp = 0,
	Stamina = 1,
	Bullet = 2,
	Melee = 3,
	Ranged = 4,
	Escape = 5
};

USTRUCT(BlueprintType)
struct FDataForWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh>  SKM;
	UPROPERTY(EditAnywhere)
	FName	HandSocket;
	UPROPERTY(EditAnywhere)
	FName	WaistSocket;
	UPROPERTY(EditAnywhere)
	int32	Value=0;
};


USTRUCT(BlueprintType)
struct FDataForItemEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montages = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayCueNotify_Static>  GCN;

};


UENUM(BlueprintType)
enum class EUIState : uint8
{
	User = 0,
	Other = 1,
	Standby = 2
};

UENUM(BlueprintType,meta = (ScriptName = "E_SpawnType"))
enum class ESpawnType : uint8
{
	None = 0,
	Item = 1,
	Enemy = 2,
	Escape =3,
	Radio =4
};

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_BODY()

public:
	FGameplayTag tag;
	int32 Count = 0;
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	None, 
	Guard,
	Observer,
	Patroller
};

UENUM(BlueprintType)
enum class EVolumeType : uint8
{
	Master,
	BG,
	Effect,
	Voice
};

UENUM(BlueprintType)
enum class EOptionType : uint8
{
	Sound,
	Resolution,
	WindowMode,
	Language,
	KeyMapping
};

UENUM(BlueprintType)
enum class ELanguage : uint8
{
	None,
	English,
	Korean
};


USTRUCT(BlueprintType)
struct FLocalizedText : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText	Text;

};