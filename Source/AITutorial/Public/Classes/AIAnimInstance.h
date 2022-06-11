// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIAnimInstance.generated.h"

/**
 * 
 */

UCLASS()
class AITUTORIAL_API UAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UAIAnimInstance();

	UPROPERTY(BlueprintReadOnly)
	class ACharacterBase* Character = nullptr;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Speed = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Direction = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bIsCrouching = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bIsInCombat = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bIsShooting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bADS = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bSprinting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool bSitting = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	bool StartTurn = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float YawActual = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Pitch = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float Yaw = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float RootYawOffset = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	FRotator HandRotation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float DistanceCurveValue = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	float MaxTurnAngle = 90.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	FName DistanceToPivot = TEXT("DistanceToPivot");

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	FName Turning = TEXT("Turning");

	void SetCharacterProperties();

	void SetPitch();

	void SetRootYawOffset();

	bool SetSprinting();

	void Recoil(float Seconds);

protected:

	float YawLastTick = 0.f;
	float YawChangeOverFrame = 0.f;
	float DistanceCurveValueLastFrame = 0.f;
	float TurnDirection = 0.f;
	float ABSRootYawOffset = 0.f;
	float DistanceCurveDifference = 0.f;
	float YawToSubtract = 0.f;
	float YawMultiplier = 0.f;

};
