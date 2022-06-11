// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/CoverActor.h"
#include "AITutorial/Public/Classes/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ACoverActor::ACoverActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACoverActor::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ACoverActor::OnOverlapEnd);

}

void ACoverActor::BeginPlay()
{
	Super::BeginPlay();
	GetComponents<UArrowComponent>(CoverSpots, true);
	Debug();
}

void ACoverActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterBase* Chr = Cast<ACharacterBase>(OtherActor);
	if (Chr && Character == nullptr)
	{
		Character = Chr;
		Available = false;
	}
}

void ACoverActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == Character)
	{
		Available = true;
		Character = nullptr;
	}
}

