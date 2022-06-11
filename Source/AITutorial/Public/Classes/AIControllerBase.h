// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class AITUTORIAL_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	class AAIManager* AIManager = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float DetectionLevel = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TimeStamp = 0.f;

	AAIControllerBase(const FObjectInitializer& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(BlueprintReadWrite)
	class ACharacterBase* Agent;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BTC;

	UPROPERTY(transient)
	class UBlackboardComponent* BBC;

	// Blackboard Key Ids
	uint8 EnemyKeyId;
	uint8 LocationKeyId;
	uint8 ContactKeyId;
	uint8 CombatRoleKeyId;
	uint8 CombatStateKeyId;
	uint8 LastStimulusLocationKeyId;
	uint8 DamagedKeyId;
	uint8 CoverActorId;
	uint8 ShootFromCoverId;

protected:

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	/** A Sight Sense config for our AI */
	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;


	FTimerHandle DetectionTimer;
	void SetDetectionLevel();
	float Rate = 1.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float DetectionThreshold = 5.f;
	AActor* Target = nullptr;
	FVector LastStimulusLocation = FVector::ZeroVector;
	
};
