// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Sit.generated.h"

/**
 * 
 */
UCLASS()
class AITUTORIAL_API UBTTask_Sit : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_Sit(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	bool Sitting = false;
	
};
