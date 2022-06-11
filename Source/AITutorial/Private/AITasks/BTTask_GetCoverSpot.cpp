// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_GetCoverSpot.h"
#include "AITutorial/Public/Classes/CoverActor.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

UBTTask_GetCoverSpot::UBTTask_GetCoverSpot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get CoverSpot";
}

EBTNodeResult::Type UBTTask_GetCoverSpot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* Cntrlr = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());

	ACoverActor* ThisCover = Cast<ACoverActor>(Cntrlr->BBC->GetValueAsObject("CoverActor"));
	AActor* Target = Cast<AActor>(Cntrlr->BBC->GetValueAsObject("TargetActor"));

	if (Cntrlr && ThisCover && ThisCover->CoverSpots.Num() > 0 && Target)
	{
		Cntrlr->BBC->SetValueAsVector("MoveToLocation", FVector::ZeroVector);
		FVector Destination = FVector::ZeroVector;
		float AngleToTarget = 0.f;

		for (auto& Arw : ThisCover->CoverSpots)
		{
			AngleToTarget = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector
			                                           (UKismetMathLibrary::GetForwardVector(Arw->GetComponentRotation()),
				                                        UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation
				                                       (Arw->GetComponentLocation(), Target->GetActorLocation()))));

			if (AngleToTarget <= DesiredAngleToTarget)
			{
				Destination = Arw->GetComponentLocation();
				break;
			}
		}

		if (Destination != FVector::ZeroVector)
		{
			Cntrlr->BBC->SetValueAsVector("MoveToLocation", Destination);
		}

	}

	return EBTNodeResult::Succeeded;
}
