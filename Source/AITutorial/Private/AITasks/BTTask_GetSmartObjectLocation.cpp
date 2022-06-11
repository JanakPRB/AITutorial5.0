// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_GetSmartObjectLocation.h"
#include "Classes/CharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Classes/SmartObject.h"
#include "Components/ArrowComponent.h"

UBTTask_GetSmartObjectLocation::UBTTask_GetSmartObjectLocation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Smart Object Location";
}

EBTNodeResult::Type UBTTask_GetSmartObjectLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!MyController || !MyBlackboard)
	{return EBTNodeResult::Failed;}

	ACharacterBase* Chr = Cast<ACharacterBase>(MyController->GetPawn());
	if (!Chr)
	{return EBTNodeResult::Failed;}

	ASmartObject* ObjRef = Cast<ASmartObject>(Chr->SmartObject);
	if (!ObjRef)
	{return EBTNodeResult::Succeeded;}

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", ObjRef->FacingDirection->GetComponentLocation());
	return EBTNodeResult::Succeeded;
}