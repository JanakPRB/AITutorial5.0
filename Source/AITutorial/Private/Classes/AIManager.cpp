// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/AIManager.h"
#include "Kismet/GameplayStatics.h"
#include "AITutorial/Public/Classes/AIControllerBase.h"
#include "AITutorial/Public/Classes/CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetmathLibrary.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AAIManager::CoverFire(bool ProvideCoverFire, ACharacterBase* Instgtr)
{
	if (!Instgtr || Defenders.Find(Instgtr) < 0) return;

	if (ProvideCoverFire)
	{
		for (auto& Agnt : Defenders)
		{
			if (Agnt != Instgtr && Agnt->ControllerRef->BBC->GetValueAsEnum("CombatState") == (uint8)ECombatState::HoldCover)
			{
				AgentProvidingCoverFire = Agnt;
				Agnt->ControllerRef->BBC->SetValueAsBool("ShootFromCover", true);
				break;
			}
		}
		return;
	}

	if (AgentProvidingCoverFire->ControllerRef->BBC->GetValueAsEnum("CombatState") == (uint8)ECombatState::HoldCover) 
		AgentProvidingCoverFire->ControllerRef->BBC->SetValueAsBool("ShootFromCover", false);
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	CreateAgentsList();
	UpdateCombatRole();
	
}

bool AAIManager::Engaged()
{
	bool ED = false;

	for (auto& Cntrlr : Agents)
	{
		if (Cntrlr->BBC->GetValueAsBool("Contact"))
		{
			ED = true;
			break;
		}

		if (Cntrlr->BBC->GetValueAsObject("TargetActor"))
		{
			ED = true;
			break;
		}

		if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - Cntrlr->TimeStamp < MaxStimulusTime_Combat)
		{
			ED = true;
			break;
		}

	}
	return ED;
}

void AAIManager::RunCombatLoop()
{
	if (Engaged())
	{
		//Approach
		if (Defenders.Num() > 0)
		{
			Defenders[ApproacherIndex]->ControllerRef->BBC->SetValueAsEnum("CombatState", (uint8)ECombatState::ApproachingCover);
			CoverFire(true, Defenders[ApproacherIndex]);
			ApproacherIndex = (ApproacherIndex + 1 <= Defenders.Num() - 1) ? ApproacherIndex+=1 : 0;
		}

		return;
	}

	NotifyAIState(EAIState::LostEnemy);
	GetWorldTimerManager().ClearTimer(CombatTimer);
}

void AAIManager::RunSearchTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("ElapsedTime Is : %f"), UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - TimeStamp));

	if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - TimeStamp < MaxStimulusTime_Search)
	{return;}

	GetWorldTimerManager().ClearTimer(SearchTimer);
	NotifyAIState(EAIState::Idle);
}

void AAIManager::CreateAgentsList()
{
	TSubclassOf<AAIControllerBase> classToFind = AAIControllerBase::StaticClass();
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, Actors);

	for (auto& Cntrlr : Actors)
	{
		AAIControllerBase* Cn = Cast<AAIControllerBase>(Cntrlr);
		if (Cn && Cn->Agent->Faction == Faction)
		{
			Agents.AddUnique(Cn);
			Cn->AIManager = this;
		}
	}
}

void AAIManager::NotifyAIState(EAIState State)
{
	for (auto& Cntrlr : Agents)
	{
		(Engaged()) ? Cntrlr->BBC->SetValueAsEnum("AIState", (uint8)EAIState::Attack) : Cntrlr->BBC->SetValueAsEnum("AIState", (uint8)State);
		Cntrlr->BBC->SetValueAsVector("LastStimulusLocation", LastStimulusLocation);
	}

	if (State == EAIState::Attack)
	{
		GetWorldTimerManager().ClearTimer(SearchTimer);
		GetWorldTimerManager().SetTimer(CombatTimer, this, &AAIManager::RunCombatLoop, ApproachDelay, true);
		return;
	}
	
	if (State == EAIState::Search)
	{
		TimeStamp = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		GetWorldTimerManager().SetTimer(SearchTimer, this, &AAIManager::RunSearchTimer, 1.f, true);
		return;
	}
}

void AAIManager::RemoveAgent(AAIControllerBase* ControllerToRemove)
{
	int32 Index = Agents.Find(ControllerToRemove);	
	if (Index < 0)
	{return;}

	Agents.RemoveSingle(ControllerToRemove);
	UpdateCombatRole();
}

void AAIManager::UpdateCombatRole()
{
	for (auto& Cntrlr : Agents)
	{
		if (Cntrlr->Agent->CombatRole == ECombatRole::Defender)
		{
			Defenders.AddUnique(Cntrlr->Agent);
		}
	}
}

