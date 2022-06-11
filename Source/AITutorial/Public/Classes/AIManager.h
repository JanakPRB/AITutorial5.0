// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITutorial/AITUTORIAL.h"
#include "GameFramework/Actor.h"
#include "AIManager.generated.h"

UCLASS()
class AITUTORIAL_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();

	UPROPERTY(BlueprintReadOnly)
	TArray<class AAIControllerBase*> Agents;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ACharacterBase*> Defenders;
	int32 ApproacherIndex = 0;

	UFUNCTION(BlueprintCallable)
	void CoverFire(bool ProvideCoverFire, ACharacterBase* Instgtr);

	class ACharacterBase* AgentProvidingCoverFire = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI")
	EFaction Faction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI")
	float MaxStimulusTime_Combat = 10.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI")
	float MaxStimulusTime_Search = 10.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI")
	float ApproachDelay = 10.f;

	UFUNCTION()
	bool Engaged();

	FTimerHandle CombatTimer;
	FTimerHandle SearchTimer;

	UFUNCTION()
	void RunCombatLoop();

	float TimeStamp = 0.f;

	UFUNCTION()
	void RunSearchTimer();

	int32 AgentIndex = 0;

	UFUNCTION()
	void UpdateCombatRole();

public:	
	
	UFUNCTION()
	void CreateAgentsList();

	UFUNCTION()
	void NotifyAIState(EAIState State);

	UFUNCTION()
	void RemoveAgent(AAIControllerBase* ControllerToRemove);

	UPROPERTY(BlueprintReadOnly)
	FVector LastStimulusLocation = FVector::ZeroVector;

};
