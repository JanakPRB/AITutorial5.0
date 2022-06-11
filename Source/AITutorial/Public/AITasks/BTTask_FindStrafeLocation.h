// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindStrafeLocation.generated.h"

/**
 * 
 */
UCLASS()
class AITUTORIAL_API UBTTask_FindStrafeLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_FindStrafeLocation(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	UEnvQuery* LocationSeekerQuery; // set the query in editor

	FEnvQueryRequest LocationSeekerQueryRequest;

	// The function that gets called when querry finished
	void LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	class AAIControllerBase* Cntrlr;

	FVector StrafeLocation = FVector::ZeroVector;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	float Distance = 500.f;

	bool IsDistanceGreaterThanX(FVector Location);
	
};
