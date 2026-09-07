// Fill out your copyright notice in the Description page of Project Settings.


#include "FlareAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UFlareAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
    
   // //// 목표 속도
   //FRotator TargetMainRotor(0.f, 360.f, 0.f);
   //FRotator TargetTailRotor(0.f, 0.f, 360.f);

   // // 0~360도로 wrap
  
   // //mMainRotorSpeed.Yaw = (double)FMath::Wrap((float)mMainRotorSpeed.Yaw, 0.f, 360.f);
   //// mTailRotorSpeed.Roll = (double)FMath::Wrap((float)mTailRotorSpeed.Roll, 0.f, 360.f);

   // //// 부드럽게 회전 속도 보간
   ///* mMainRotorSpeed = FMath::RInterpTo(mMainRotorSpeed, TargetMainRotor, DeltaSeconds, 2.f);
   // mTailRotorSpeed = FMath::RInterpTo(mTailRotorSpeed, TargetTailRotor, DeltaSeconds, 2.f);*/

   // mMainRotorSpeed.Yaw += 10.f;
   // mTailRotorSpeed.Roll += 10.f;

    //mMainRotorSpeed = FMath::RInterpTo(mMainRotorSpeed, TargetMainRotor, DeltaSeconds, 2.f);
    //mTailRotorSpeed = FMath::RInterpTo(mTailRotorSpeed, TargetTailRotor, DeltaSeconds, 2.f); 
}




