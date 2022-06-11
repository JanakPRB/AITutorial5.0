// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_RotateWithInterp.h"
#include "Classes/CharacterBase.h"
#include "AIController.h"
#include "Classes/SmartObject.h"
#include "Components/ArrowComponent.h"
#include "Kismet/Kismetmathlibrary.h"
#include "Kismet/GameplayStatics.h"

UBTTask_RotateWithInterp::UBTTask_RotateWithInterp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Rotate With Interp";
}

EBTNodeResult::Type UBTTask_RotateWithInterp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	Chr->SetActorRotation(UKismetMathLibrary::RInterpTo(Chr->GetActorRotation(), ObjRef->FacingDirection->GetComponentRotation(), UGameplayStatics::GetWorldDeltaSeconds(Chr)*4, 25.f));
	return EBTNodeResult::Succeeded;
}
