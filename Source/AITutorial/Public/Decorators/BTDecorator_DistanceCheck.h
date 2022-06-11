// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_DistanceCheck.generated.h"

/**
 * 
 */
UCLASS()
class AITUTORIAL_API UBTDecorator_DistanceCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:

	UBTDecorator_DistanceCheck(const FObjectInitializer& ObjectInitializer);

	// Return True if the Current Distance Is GREATER THAN This Distance
	UPROPERTY(EditAnywhere, Category = Condition)
	float TestDistance = 1000.f;
	
};
