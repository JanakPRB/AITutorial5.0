// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITutorial/AITUTORIAL.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_OverrideAIState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ERunMode : uint8
{
	ThisAgent         UMETA(DisplayName = "This Agent"),
	AllAgents         UMETA(DisplayName = "All Agents"),
};

UCLASS()
class AITUTORIAL_API UBTTask_OverrideAIState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_OverrideAIState(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	EAIState DesiredState;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	ERunMode RunMode;

};
