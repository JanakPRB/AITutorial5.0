// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CoverActor.generated.h"

/**
 * 
 */
UCLASS()
class AITUTORIAL_API ACoverActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ACoverActor();

	UPROPERTY(BlueprintReadWrite)
	class ACharacterBase* Character;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	TArray<class UArrowComponent*> CoverSpots;

	bool Available = true;

	UFUNCTION(BlueprintImplementableEvent)
	void Debug();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* Collider;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			                                                                            int32 OtherBodyIndex);
	
};
