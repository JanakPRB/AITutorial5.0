// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_SelectTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AITutorial/Public/Classes/CharacterBase.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"

UBTTask_SelectTarget::UBTTask_SelectTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Select Target";
}

EBTNodeResult::Type UBTTask_SelectTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cntrlr = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());

	if (Cntrlr && EnemySeekerQuery)
	{
		EnemySeekerQueryRequest = FEnvQueryRequest(EnemySeekerQuery, Cntrlr->Agent);
		EnemySeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTask_SelectTarget::EnemySeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_SelectTarget::EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	BestTarget = nullptr;
	Cntrlr->BBC->SetValueAsObject("TargetActor", nullptr);

	float CurrentBestScore = 0.f;
	int32 Index = 0;
	TArray<AActor*> AllDetectedActors;
	Result->GetAllAsActors(AllDetectedActors);

	for (auto& DetectedActor : AllDetectedActors)
	{
		ACharacterBase* Chr = Cast<ACharacterBase>(DetectedActor);
		if (Chr && Chr->IsHostile(Cntrlr->Agent) && !Chr->Dead)
		{
			if (Result->GetItemScore(Index) > CurrentBestScore && Result->GetItemScore(Index) >0.f)
			{
				BestTarget = Chr;
				CurrentBestScore = Result->GetItemScore(Index);
			}

			// Increment Index
			Index++;
		}
	}

	if (BestTarget)
	{Cntrlr->BBC->SetValueAsObject("TargetActor", BestTarget);}

}