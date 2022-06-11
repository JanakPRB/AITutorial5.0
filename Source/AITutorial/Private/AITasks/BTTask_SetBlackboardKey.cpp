// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_SetBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"

UBTTask_SetBlackboardKey::UBTTask_SetBlackboardKey(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Set Blackboard Key";
}

EBTNodeResult::Type UBTTask_SetBlackboardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (MyController)
	{
		MyController->BBC->SetValueAsBool("Damaged", Damage);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}