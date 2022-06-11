// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_FindStrafeLocation.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"
#include "AITutorial/Public/Classes/CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Classes/AIManager.h"

UBTTask_FindStrafeLocation::UBTTask_FindStrafeLocation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Find Strafe Location";
}

EBTNodeResult::Type UBTTask_FindStrafeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cntrlr = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());

	if (Cntrlr && LocationSeekerQuery)
	{
		LocationSeekerQueryRequest = FEnvQueryRequest(LocationSeekerQuery, Cntrlr->Agent);
		LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTask_FindStrafeLocation::LocationSeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_FindStrafeLocation::LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	int32 Index = 0;
	float CurrentBestScore = 0;
	TArray<FVector> Locations;
	Result->GetAllAsLocations(Locations);

	for (auto& Loc : Locations)
	{
		if (IsDistanceGreaterThanX(Loc) && Result->GetItemScore(Index) > CurrentBestScore)
		{
			StrafeLocation = Loc;
			CurrentBestScore = Result->GetItemScore(Index);
		}

		Index++;
	}

	Cntrlr->BBC->SetValueAsVector("MoveToLocation", StrafeLocation);
}

bool UBTTask_FindStrafeLocation::IsDistanceGreaterThanX(FVector Location)
{
	if (Cntrlr->AIManager->Agents.Num() <= 1) return true;

	bool ConsiderLocation = true;
	for (auto& Agnt : Cntrlr->AIManager->Agents)
	{
		const float CalculatedDistance = (Location - Agnt->Agent->GetActorLocation()).Size();
		if (CalculatedDistance <= Distance)
		{
			ConsiderLocation = false;
			break;
		}
	}
	
	return ConsiderLocation;
}