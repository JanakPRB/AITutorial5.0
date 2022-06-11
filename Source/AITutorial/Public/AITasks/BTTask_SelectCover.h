// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SelectCover.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECoverType : uint8
{
	Approach          UMETA(DisplayName = "Approach"),
	Reposition        UMETA(DisplayName = "Reposition"),
	Retreat           UMETA(DisplayName = "Retreat")
};

UCLASS()
class AITUTORIAL_API UBTTask_SelectCover : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_SelectCover(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	UEnvQuery* CoverSeekerQuery; // set the query in editor

	FEnvQueryRequest CoverSeekerQueryRequest;

	// The function that gets called when querry finished
	void CoverSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	class AAIControllerBase* Cntrlr;

	class ACoverActor* BestCover;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Blackboard")
	float DesiredDirectness = 0.f;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Blackboard")
	float DesiredDistance = 500.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	ECoverType RunMode;
	
};
