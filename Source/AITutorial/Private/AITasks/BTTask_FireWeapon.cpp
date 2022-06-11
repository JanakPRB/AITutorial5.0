// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_FireWeapon.h"
#include "AITutorial/Public/Classes/CharacterBase.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FireWeapon::UBTTask_FireWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Fire Weapon";
}

EBTNodeResult::Type UBTTask_FireWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (!MyController)
	{return EBTNodeResult::Failed;}

	Chr = Cast<ACharacterBase>(MyController->GetPawn());
	if (!Chr)
	{return EBTNodeResult::Failed;}

	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	auto MyID = MyBlackboard->GetKeyID(BlackboardKey.SelectedKeyName);

	ACharacterBase* EnemyActor = Cast<ACharacterBase>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(MyID));
	if (!EnemyActor || EnemyActor->Dead || !ShouldFire) 
	{
		Shoot(false);
		return EBTNodeResult::Succeeded;
	}

	
	FHitResult OutHit = MyController->Agent->CapsuleTrace();
	ACharacterBase* HitActor = Cast<ACharacterBase>(OutHit.GetActor());
	if (HitActor && HitActor->Faction == MyController->Agent->Faction)
	{
		Shoot(false);
		return EBTNodeResult::Succeeded;
	}

	Shoot(true);
	return EBTNodeResult::Succeeded;
}

void UBTTask_FireWeapon::Shoot(bool Newbool)
{
	Chr->ToggleADS(Newbool);
	if (Newbool)
	{
		if (!Chr->FireHandle.IsValid())
		{Chr->StartWeaponFire();}
		return;
	}
	Chr->StopWeaponFire();
}
