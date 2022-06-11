// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_Sit.h"
#include "Classes/CharacterBase.h"
#include "AIController.h"

UBTTask_Sit::UBTTask_Sit(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Toggle Sitting";
}

EBTNodeResult::Type UBTTask_Sit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!MyController || !MyBlackboard)
	{return EBTNodeResult::Failed;}

	ACharacterBase* Chr = Cast<ACharacterBase>(MyController->GetPawn());
	if (!Chr)
	{return EBTNodeResult::Failed;}

	Chr->ToggleSitting(Sitting);
	return EBTNodeResult::Succeeded;
}