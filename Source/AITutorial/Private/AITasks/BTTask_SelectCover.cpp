// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_SelectCover.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"
#include "AITutorial/Public/Classes/CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AITutorial/Public/Classes/CoverActor.h"

UBTTask_SelectCover::UBTTask_SelectCover(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Select Cover";
}

EBTNodeResult::Type UBTTask_SelectCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cntrlr = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());

	if (Cntrlr && CoverSeekerQuery)
	{
		CoverSeekerQueryRequest = FEnvQueryRequest(CoverSeekerQuery, Cntrlr->Agent);
		CoverSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTask_SelectCover::CoverSeekerQueryFinished);
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_SelectCover::CoverSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	BestCover = nullptr;
	AActor* Target = Cast<AActor>(Cntrlr->BBC->GetValueAsObject("TargetActor"));

	float CurrentBestScore = 0.f;
	int32 Index = 0;
	TArray<AActor*> AllDetectedActors;
	Result->GetAllAsActors(AllDetectedActors);

	if (RunMode == ECoverType::Approach)
	{
		for (auto& DetectedActor : AllDetectedActors)
		{
			ACoverActor* ThisCover = Cast<ACoverActor>(DetectedActor);
			if (ThisCover && ThisCover->GetDistanceTo(Target) >= DesiredDistance && ThisCover->Available && ThisCover->Character == nullptr)
			{
				const float CalculatedDirectness = (Cntrlr->Agent->GetDistanceTo(Target) - ThisCover->GetDistanceTo(Target)) /
					                                                                 Cntrlr->Agent->GetDistanceTo(ThisCover);

				if (Result->GetItemScore(Index) > CurrentBestScore && CalculatedDirectness > DesiredDirectness)
				{
					BestCover = ThisCover;
					CurrentBestScore = Result->GetItemScore(Index);
				}
			}
			Index++;
		}
	}

	if (RunMode == ECoverType::Reposition)
	{
		if (Result->GetItemScore(Index) > CurrentBestScore)
		{
			for (auto& DetectedActor : AllDetectedActors)
			{
				ACoverActor* ThisCover = Cast<ACoverActor>(DetectedActor);
				if (ThisCover && ThisCover->Available && ThisCover->Character == nullptr && Result->GetItemScore(Index) > CurrentBestScore)
				{
					BestCover = ThisCover;
					CurrentBestScore = Result->GetItemScore(Index);
				}
				Index++;
			}
		}
	}

	if (RunMode == ECoverType::Retreat)
	{
		for (auto& DetectedActor : AllDetectedActors)
		{
			ACoverActor* ThisCover = Cast<ACoverActor>(DetectedActor);
			if (ThisCover && ThisCover->GetDistanceTo(Target) >= DesiredDistance && ThisCover->Available && ThisCover->Character == nullptr)
			{
				const float CalculatedDirectness = (Cntrlr->Agent->GetDistanceTo(Target) - ThisCover->GetDistanceTo(Target)) /
					                                                                 Cntrlr->Agent->GetDistanceTo(ThisCover);

				if (Result->GetItemScore(Index) > CurrentBestScore && CalculatedDirectness < DesiredDirectness)
				{
					BestCover = ThisCover;
					CurrentBestScore = Result->GetItemScore(Index);
				}
			}
			Index++;
		}
	}

	if (BestCover)
	{
		ACoverActor* PreviousCover = Cast<ACoverActor>(Cntrlr->BBC->GetValueAsObject("CoverActor"));
		if (PreviousCover && PreviousCover != BestCover)
		{
			PreviousCover->Available = true;
			PreviousCover->Character = nullptr;
		}

		Cntrlr->BBC->SetValueAsObject("CoverActor", BestCover);
		BestCover->Available = false;
		BestCover->Character = Cntrlr->Agent;
	}
}
