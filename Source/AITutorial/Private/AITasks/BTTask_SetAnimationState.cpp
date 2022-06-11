// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_SetAnimationState.h"
#include "Classes/CharacterBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_SetAnimationState::UBTTask_SetAnimationState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Set Animation State";
	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_SetAnimationState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	ACharacterBase* Chr = Cast<ACharacterBase>(MyController->GetPawn());
	if (MyController && Chr)
	{
		Chr->ToggleCombat(Combat);
		Chr->ToggleCrouch(Crouch);
		Chr->ToggleSprinting(Sprint);

		const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		auto MyID = MyBlackboard->GetKeyID(BlackboardKey.SelectedKeyName);

		ACharacterBase* EnemyActor = Cast<ACharacterBase>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(MyID));
		if (EnemyActor)
		{
			(Focus) ? MyController->SetFocus(EnemyActor) : MyController->ClearFocus(EAIFocusPriority::LastFocusPriority);
			Chr->ToggleADS(ADS);
			return EBTNodeResult::Succeeded;
		}
		MyController->ClearFocus(EAIFocusPriority::LastFocusPriority);
		Chr->ToggleADS(false);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
