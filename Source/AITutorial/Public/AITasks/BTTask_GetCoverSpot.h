// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetCoverSpot.generated.h"

/**
 * 
 */
UCLASS()
class AITUTORIAL_API UBTTask_GetCoverSpot : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_GetCoverSpot(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	float DesiredAngleToTarget = 45.f;
	
};
