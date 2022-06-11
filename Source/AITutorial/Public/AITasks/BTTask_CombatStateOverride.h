// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITutorial/AITUTORIAL.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CombatStateOverride.generated.h"

/**
 * 
 */
UCLASS()
class AITUTORIAL_API UBTTask_CombatStateOverride : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_CombatStateOverride(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	ECombatState DesiredState;
	
};
