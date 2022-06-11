// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SelectTarget.generated.h"

/**
 * 
 */
UCLASS()
class AITUTORIAL_API UBTTask_SelectTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_SelectTarget(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	UEnvQuery* EnemySeekerQuery; // set the query in editor

	FEnvQueryRequest EnemySeekerQueryRequest;

	// The function that gets called when querry finished
	void EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	class AAIControllerBase* Cntrlr;

	class ACharacterBase* BestTarget;
	
};
