// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_CombatStateOverride.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

UBTTask_CombatStateOverride::UBTTask_CombatStateOverride(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "CombatState Override";
}

EBTNodeResult::Type UBTTask_CombatStateOverride::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (!MyController || !MyController->AIManager)
	{return EBTNodeResult::Failed;}

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>("CombatState", (uint8)DesiredState);
	return EBTNodeResult::Succeeded;
}
