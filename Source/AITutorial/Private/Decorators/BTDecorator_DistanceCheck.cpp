// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTDecorator_DistanceCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIController.h"

UBTDecorator_DistanceCheck::UBTDecorator_DistanceCheck(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Distance Check";
	bCreateNodeInstance = true;
	// accept only actors	
	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}

bool UBTDecorator_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	bool OutsideRange = false;

	if (!MyController || !MyBlackboard)
	{return OutsideRange;}

	auto MyID = MyBlackboard->GetKeyID(BlackboardKey.SelectedKeyName);

	UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(MyID);
	AActor* TargetActor = Cast<AActor>(KeyValue);
	if (!TargetActor)
	{return OutsideRange;}

	if (MyController->GetPawn()->GetDistanceTo(TargetActor) >= TestDistance)
	{OutsideRange = true;}

	return OutsideRange;
}
