// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmartObject.generated.h"

UCLASS()
class AITUTORIAL_API ASmartObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmartObject();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	class UBillboardComponent* Billboard;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* FacingDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	// Behavior Tree Asset
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* SubTree; 

};
