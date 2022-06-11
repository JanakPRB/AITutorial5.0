// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/AIControllerBase.h"
#include "Classes/SmartObject.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AITutorial/Public/Classes/CharacterBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Hearing.h"
#include "AITutorial/AITUTORIAL.h"
#include "Kismet/KismetmathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AITutorial/Public/Classes/AIManager.h"
#include "GameFramework/CharacterMovementComponent.h"


AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer)
{
	BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("behaviorTree Compponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	//Create a Sight And Hearing Sense
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	Sight->SightRadius = 2000.f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;

	Hearing->HearingRange = 2000.f;

	//Tell the senses to detect everything
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;

	//Register the sight sense to our Perception Component
	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->ConfigureSense(*Hearing);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (!Agent)
	{
		ACharacterBase* Chr = Cast<ACharacterBase>(GetPawn());
		if (!Chr)
		{return;}
		Agent = Chr;
		Agent->ControllerRef = this;
	}

	if (Agent->SmartObject)
	{
		FGameplayTag SubTag;
		BTC->SetDynamicSubtree(SubTag, Agent->SmartObject->SubTree);
	}

	BBC->SetValueAsEnum("CombatRole", (uint8)Agent->CombatRole);
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACharacterBase* Chr = Cast<ACharacterBase>(InPawn);
	if (Chr != nullptr && Chr->TreeAsset != nullptr)
	{
		Agent = Chr;
		Agent->ControllerRef = this;

		// Initialize the Blackboard
		BBC->InitializeBlackboard(*Chr->TreeAsset->BlackboardAsset);

		// Set Blackboard Key IDs
		EnemyKeyId = BBC->GetKeyID("TargetActor");
		LocationKeyId = BBC->GetKeyID("MoveToLocation");
		ContactKeyId = BBC->GetKeyID("Contact");
		CombatRoleKeyId = BBC->GetKeyID("CombatRole");
		CombatStateKeyId = BBC->GetKeyID("CombatState");
		LastStimulusLocationKeyId = BBC->GetKeyID("LastStimulusLocation");
		DamagedKeyId = BBC->GetKeyID("Damaged");
		CoverActorId = BBC->GetKeyID("CoverActor");
		ShootFromCoverId = BBC->GetKeyID("ShootFromCover");

		// Start The BehaviorTree.
		BTC->StartTree(*Chr->TreeAsset);
	}

	//Register the OnPerceptionUpdated function to fire whenever the AIPerception get's updated
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnPerception);
}

void AAIControllerBase::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
	{
		ACharacterBase* Chr = Cast<ACharacterBase>(Actor);
		if (Chr && Chr->IsHostile(Agent))
		{
			BBC->SetValueAsBool("Contact", Stimulus.WasSuccessfullySensed());

			if (BBC->GetValueAsEnum("AIState") != (uint8)EAIState::Attack)
			{
				Agent->GetCharacterMovement()->StopActiveMovement();
				BBC->SetValueAsObject("TargetActor", Actor);
			}
			
			Target = Actor;
			LastStimulusLocation = Stimulus.StimulusLocation;
			if(AIManager) AIManager->LastStimulusLocation = LastStimulusLocation;
			TimeStamp = UKismetSystemLibrary::GetGameTimeInSeconds(Agent);

		}

		if (!GetWorldTimerManager().IsTimerActive(DetectionTimer) && BBC->GetValueAsBool("Contact") && BBC->GetValueAsEnum("AIState") == (uint8)EAIState::Idle)
		{
			DetectionLevel = 0.f;
			Agent->UpdateWidgetVis(true);
			GetWorldTimerManager().SetTimer(DetectionTimer, this, &AAIControllerBase::SetDetectionLevel, Rate, true, 0.f);
		}

		return;
	}

	if (BBC->GetValueAsEnum("AIState") == (uint8)EAIState::Attack) return;

	ACharacterBase* Chr = Cast<ACharacterBase>(Actor);
	if (Chr && Chr->IsHostile(Agent))
	{
		BBC->SetValueAsEnum("AIState", (uint8)EAIState::Investigate);
		BBC->SetValueAsVector("LastStimulusLocation", Stimulus.StimulusLocation);
	}
}

void AAIControllerBase::SetDetectionLevel()
{
	if (!Target || !BBC->GetValueAsBool("Contact"))
	{
		if (BBC->GetValueAsEnum("AIState") != (uint8)EAIState::Idle)
		{
			GetWorldTimerManager().ClearTimer(DetectionTimer);
			Agent->UpdateWidgetVis(false);
			return;
		}

		if (DetectionLevel > 0.f)
		{
			DetectionLevel -= 1;
			return;
		}
		GetWorldTimerManager().ClearTimer(DetectionTimer);
		Agent->UpdateWidgetVis(false);

		return;
	}

	const float Distance = GetPawn()->GetDistanceTo(Target);
	Rate = (Distance <= 500.f) ? 1.f : 2.f;
	DetectionLevel += 1;

	if (DetectionLevel >= DetectionThreshold)
	{
		if(AIManager) AIManager->NotifyAIState(EAIState::Attack);
		GetWorldTimerManager().ClearTimer(DetectionTimer);
		Agent->UpdateWidgetVis(false);
		return;
	}

	if (DetectionLevel >= DetectionThreshold / 2)
	{
		BBC->SetValueAsEnum("AIState", (uint8)EAIState::Investigate);
		BBC->SetValueAsVector("LastStimulusLocation", LastStimulusLocation);
	}
}
