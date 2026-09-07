//////////////////////////
/*
    생성 날짜 : 2025.09.02
    생성자   : 김다해
    기능     : 아이템 타입 및 효과 정의 헤더 파일
               - 모든 프로젝트에서 쓰는 아이템 관련 열거형(ENUM)과 효과 구조체(Struct) 정의.
               - 블루프린트에서도 바로 쓰게 BlueprintType/BlueprintReadOnly 지정.
    @ToDo    : 필요 시 무기/탄종 타입 확장, 효과 파라미터(값/지속시간 외 곱/가중치 등) 추가.
*/
//////////////////////////

#pragma once
#include "CoreMinimal.h"              
#include "ItemTypes.generated.h"    

//아이템 대분류: 인벤/툴팁/필터에 사용
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
    Weapon,       // 무기류(원/근)
    Consumable,   // 회복/버프 등 소모품
    Ammo,         // 탄약/화살 같은 보급품
    Extraction,   // 탈출 관련(신호탄/무전기/지도 등)
    Utility       // 잡동사니/기타
};

// 무기 세부 분류 : 장착/데미지/로직 등에 사용
UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
    None,    // 무기 아님
    Ranged,  // 원거리(권총/라이플 등)
    Melee    // 근접(칼/도끼 등)
};

// 탄종 : 탄약 아이템과 무기 장착/재장전 로직에 사용
// 불필요
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
    None,        // 탄약 사용 안함
    Pistol9mm,   // 9mm
    Rifle556,    // 5.56
    Shotgun12g   // 12게이지
};

// 사용 방식: 입력 UX/애님/상호작용 플로우 분기
UENUM(BlueprintType)
enum class EUseMethod : uint8
{
    Instant,      // 즉시 사용(붕대/음료)
    Hold,         // 홀드/캐스팅(무전기 사용 등)
    EquipAndUse,  // 장비 후 사용(무기 휘두르기/사격)
    PlaceWorld    // 월드에 배치(신호탄)
};

// 효과 타입 : 아이템의 효과 종류
UENUM(BlueprintType)
enum class EItemEffectType : uint8
{
    None,               // 효과 없음(플레이스홀더)
    RestoreHealth,      // 체력 회복(+IntValue)
    RestoreStamina,     // 스태미너 회복(+IntValue)
    SpendAmmoOnReload,  // 재장전 시 탄약 차감(IntValue=장전 수)
    RevealExtraction,   // 탈출 위치/시간 표시
    CallHelicopter,     // 헬기 호출/레이드 트리거
    ShowMapUI,          // 인벤에 있으면 지도 UI 활성화
    TeamPing            // 팀 알림/핑/자막 등 브로드캐스트
};

// 효과 파라미터 : 효과 한줄 쓰기 (타입+값+지속 등)
USTRUCT(BlueprintType)
struct FItemEffect
{
    GENERATED_BODY()

    // 어떤 효과인지(회복/탈출표시/헬기호출 …)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EItemEffectType Type = EItemEffectType::None;

    //  - 회복량, 충전 수, 반경, 스택 수 같은 “양/수치”
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "양/수치(회복량, 탄 수, 반경 등)"))
    int32 IntValue = 0;

    // 지속시간/쿨다운/카운트다운(초):
    //  - 버프 지속, 사용 쿨타임, 표시 유지시간 같은 시간값
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "지속시간/쿨다운/카운트다운(초)"))
    float Duration = 0.f;

    // 팀 브로드캐스트 여부:
    //  - true면 팀원에게도 알림/마커/자막 등 전파
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "팀 알림/마커 표시 같은 옵션"))
    bool bNotifyTeam = false;
};