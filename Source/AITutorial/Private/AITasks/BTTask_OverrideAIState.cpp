// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_OverrideAIState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AITutorial/Public/Classes/AIManager.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

UBTTask_OverrideAIState::UBTTask_OverrideAIState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "AIState Override";
}

EBTNodeResult::Type UBTTask_OverrideAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (!MyController || !MyController->AIManager)
	{return EBTNodeResult::Failed;}

	(RunMode == ERunMode::ThisAgent) ? (OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>("AIState", (uint8)DesiredState)) : (MyController->AIManager->NotifyAIState(DesiredState));
	return EBTNodeResult::Succeeded;
}
