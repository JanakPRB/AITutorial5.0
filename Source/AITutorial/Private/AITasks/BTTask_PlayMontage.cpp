// Fill out your copyright notice in the Description page of Project Settings.


#include "AITasks/BTTask_PlayMontage.h"
#include "Classes/CharacterBase.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "Engine.h"

UBTTask_PlayMontage::UBTTask_PlayMontage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Play Montage";
	bNonBlocking = false;

	TimerDelegate = FTimerDelegate::CreateUObject(this, &UBTTask_PlayMontage::OnAnimationTimerDone);
	PreviousAnimationMode = EAnimationMode::AnimationBlueprint;
}

EBTNodeResult::Type UBTTask_PlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const MyController = OwnerComp.GetAIOwner();
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	// reset timer handle
	TimerHandle.Invalidate();
	MyOwnerComp = &OwnerComp;

	if (RunMode == EMontageMode::FromSelection)
	{
		if (MontageToPlay && MyController && MyController->GetPawn())
		{
			ACharacter* const MyCharacter = Cast<ACharacter>(MyController->GetPawn());
			if (MyCharacter)
			{
				CachedSkelMesh = MyCharacter->GetMesh();

				UAnimInstance* AnimInstance = Cast<UAnimInstance>(CachedSkelMesh->GetAnimInstance());
				if (AnimInstance)
				{
					if (AnimInstance->IsAnyMontagePlaying() == false)
					{
						AnimInstance->Montage_Play(MontageToPlay, PlayRate);
						const float FinishDelay = MontageToPlay->GetMaxCurrentTime();
						if (bNonBlocking == false && FinishDelay > 0)
						{
							if (OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Bool>("Contact") == false)
							{
								MyController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, FinishDelay, /*bLoop=*/false);
								Result = EBTNodeResult::InProgress;
							}
							else
							{
								AbortTask(OwnerComp, NodeMemory);
							}

						}
						else
						{
							Result = EBTNodeResult::Succeeded;
						}

					}
				}
			}
		}
	}
	else
	{
		ACharacterBase* const MyCharacter = Cast<ACharacterBase>(MyController->GetPawn());
		if (MyCharacter)
		{
			CachedSkelMesh = MyCharacter->GetMesh();

			UAnimInstance* AnimInstance = Cast<UAnimInstance>(CachedSkelMesh->GetAnimInstance());
			if (AnimInstance)
			{
				if (AnimInstance->IsAnyMontagePlaying() == false && MyCharacter->IdleVariations.Num() > 0)
				{
					UAnimMontage* NewMontage = MyCharacter->ReturnRandomVariation();
					AnimInstance->Montage_Play(NewMontage, PlayRate);
					const float FinishDelay = NewMontage->GetMaxCurrentTime();
					if (bNonBlocking == false && FinishDelay > 0)
					{
						if (OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Bool>("Contact") == false)
						{
							MyController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, FinishDelay, /*bLoop=*/false);
							Result = EBTNodeResult::InProgress;
						}
						else
						{
							AbortTask(OwnerComp, NodeMemory);
						}

					}
					else
					{
						Result = EBTNodeResult::Succeeded;
					}

				}
			}
		}
	}

	return Result;
}

EBTNodeResult::Type UBTTask_PlayMontage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const MyController = OwnerComp.GetAIOwner();

	if (MontageToPlay && MyController && TimerHandle.IsValid())
	{
		MyController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	TimerHandle.Invalidate();
	return EBTNodeResult::Aborted;
}

void UBTTask_PlayMontage::OnAnimationTimerDone()
{
	if (MyOwnerComp)
	{
		FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
	}
}
