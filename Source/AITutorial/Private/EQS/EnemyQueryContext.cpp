// Fill out your copyright notice in the Description page of Project Settings.


#include "EQS/EnemyQueryContext.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"

void UEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AAIControllerBase* Cntrlr = Cast<AAIControllerBase>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));
	if (Cntrlr)
	{
		AActor* Target = Cast<AActor>(Cntrlr->BBC->GetValueAsObject("TargetActor"));
		if (Target)
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Target);
		}
	}
}
