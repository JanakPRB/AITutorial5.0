// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PlayMontage.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMontageMode : uint8
{
	FromSelection     UMETA(DisplayName = "From Selection"),
	FromActor         UMETA(DisplayName = "From Actor")
};

UCLASS()
class AITUTORIAL_API UBTTask_PlayMontage : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_PlayMontage(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintreadOnly, Category = "Blackboard")
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnyWhere, BlueprintreadOnly, Category = "Blackboard")
	float PlayRate = 1.f;

	UPROPERTY(EditAnyWhere, BlueprintreadOnly, Category = "Blackboard")
	uint32 bNonBlocking : 1;

	UPROPERTY(EditAnyWhere, BlueprintreadOnly, Category = "Blackboard")
	EMontageMode RunMode;

	UPROPERTY()
	UBehaviorTreeComponent* MyOwnerComp;

	UPROPERTY()
	USkeletalMeshComponent* CachedSkelMesh;

	EAnimationMode::Type PreviousAnimationMode;

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	void OnAnimationTimerDone();
	
};
