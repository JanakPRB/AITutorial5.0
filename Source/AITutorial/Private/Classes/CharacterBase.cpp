// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Classes/AIControllerBase.h"
#include "BrainComponent.h"
#include "Classes/AIManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Classes/CoverActor.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
}

// Custom View Target For AIPerception Component
bool ACharacterBase::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, 
	                               float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const
{
	static const FName NAME_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));

	FHitResult HitResult;
	FVector SocketLocation = GetMesh()->GetSocketLocation(PerceptionTarget);

	const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, SocketLocation,
		                                                            FCollisionObjectQueryParams(ECC_TO_BITFIELD
		                                                           (ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)),
		                                                            FCollisionQueryParams(NAME_AILineOfSight, true,
			                                                        IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHitSocket == false || (HitResult.GetActor() && HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = SocketLocation;
		OutSightStrength = 1;

		return true;
	}

	const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation(),
		                                                      FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) |
			                                                  ECC_TO_BITFIELD(ECC_WorldDynamic)), FCollisionQueryParams
			                                                 (NAME_AILineOfSight, true, IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHit == false || (HitResult.GetActor() && HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = GetActorLocation();
		OutSightStrength = 1;

		return true;
	}

	OutSightStrength = 0;
	return false;
}

void ACharacterBase::MoveForward(float Val)
{
	if ((Controller == NULL) || (Val == 0.0f))
	{
		return;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Val);
}

void ACharacterBase::MoveRight(float Val)
{
	if ((Controller == NULL) || (Val == 0.0f))
	{
		return;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Val);
}

void ACharacterBase::LookUp(float Val)
{
	if (Val == 0.f)
	{
		return;
	}
	AddControllerPitchInput(Val);
}

void ACharacterBase::Turn(float Val)
{
	if (Val == 0.f)
	{
		return;
	}
	AddControllerYawInput(Val);
}

void ACharacterBase::UseAmmo()
{
	ShotsFired++;
	if (ShotsFired >= MagSize)
	{
		PlayMontage(ReloadMontage, 1.5);
		ShotsFired = 0;
		if (ControllerRef)
		{
			ControllerRef->BBC->SetValueAsBool("ShootFromCover", false);
			ControllerRef->AIManager->CoverFire(true, this);
		}
	}
}

FHitResult ACharacterBase::TraceProvider(FVector Start, FVector End)
{
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	return OutHit;
}

void ACharacterBase::ToggleCombat(const bool Newbool)
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
	AnimValues.bIsInCombat = Newbool;
	bUseControllerRotationYaw = Newbool;
	GetCharacterMovement()->bOrientRotationToMovement = !Newbool;
	FName NewSocket = Newbool ? "hand_rSocket" : "spine_03Socket";
	AttachWeapon(Weapon, NewSocket);
	GetCharacterMovement()->MaxWalkSpeed = (Newbool) ? 187.f : 94.f;
	
}

void ACharacterBase::ToggleCrouch(const bool Newbool)
{
	AnimValues.bIsCrouching = Newbool;
	const float Speed = AnimValues.bIsInCombat ? 187.f : WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = (Newbool) ? CrouchedWalkSpeed : Speed;
}

void ACharacterBase::ToggleADS(const bool Newbool)
{
	AnimValues.bADS = Newbool;
}

void ACharacterBase::ToggleSprinting(bool Newbool)
{
	if (Newbool)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		return;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = (AnimValues.bIsInCombat) ? 187.f : WalkSpeed;
}

void ACharacterBase::AttachWeapon(USkeletalMeshComponent* WeaponMesh, const FName SocketName)
{
	if (WeaponMesh)
		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

void ACharacterBase::StartWeaponFire()
{
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		return;

	if (!AnimValues.bIsInCombat)
		return;

	ToggleADS(true);
	ToggleSprinting(false);
	AnimValues.bIsShooting = true;
	SpawnMuzzleFlash();

	FVector EyesLoc;
	FRotator EyesRot;
	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	////////////////////////////////////////////////TO DO : Implement BulletSpred/////////////////////////////////////////////////////////////////////////

	const FVector Spread = (FVector(UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f), UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f), 
							UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f)));
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	const FVector End = (BulletSpread) ? (EyesRot.Vector() * 2000.f) + EyesLoc * Spread : (EyesRot.Vector() * 2000.f) + EyesLoc;

	FHitResult HitInfo = TraceProvider(EyesLoc, End);
	UseAmmo();

	if (!HitInfo.bBlockingHit)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(this, BulletImpact, HitInfo.Location, FRotator(0.f, 0.f, 0.f), true);
	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), BaseDamage, HitInfo.ImpactPoint, HitInfo, this->GetController(), this, nullptr);

	if (FireHandle.IsValid())
	{return;}
	GetWorldTimerManager().SetTimer(FireHandle, this, &ACharacterBase::StartWeaponFire, FireRate, true, 0.f);
}

void ACharacterBase::SpawnMuzzleFlash()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Weapon, "MuzzleFlash");
}

void ACharacterBase::StopWeaponFire()
{
	GetWorldTimerManager().ClearTimer(FireHandle);
	AnimValues.bIsShooting = false;
	if (ControllerRef) ControllerRef->BBC->SetValueAsBool("ShootFromCover", false);
}

void ACharacterBase::PlayMontage(UAnimMontage* Montage, float Rate)
{
	if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{GetMesh()->GetAnimInstance()->Montage_Play(Montage, Rate);}
}

UAnimMontage* ACharacterBase::ReturnRandomVariation()
{
	int i = UKismetMathLibrary::RandomIntegerInRange(0, IdleVariations.Num() - 1);
	return IdleVariations[i];
}

void ACharacterBase::ToggleSitting(bool Newbool)
{
	GetCharacterMovement()->StopMovementImmediately();
	AnimValues.bIsSitting = Newbool;
}

void ACharacterBase::MakeANoise(FVector Location)
{
	MakeNoise(1.f, nullptr, Location, 0.f, "");
}

FHitResult ACharacterBase::CapsuleTrace()
{
	FHitResult OutHit;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);

	FVector EyesLoc;
	FRotator EyesRot;
	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	const FVector End = (EyesRot.Vector() * 2000.f) + EyesLoc + FVector(0.f, 0.f, 120.f);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), EyesLoc, End, 20.f, ETraceTypeQuery::TraceTypeQuery_MAX, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FColor::Green);
	return OutHit;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "spine_03Socket");
	UpdateWidgetRef();
}

float ACharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* Eventinstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, Eventinstigator, DamageCauser);

	if (ControllerRef)
	{
		ACharacterBase* Damager = Cast<ACharacterBase>(DamageCauser);
		if (Damager && Damager->Faction == Faction)
		{
			Damager->StopWeaponFire();
			return 0.f;
		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(this, BloodFX, GetActorLocation());

	if (Health <= 0.f)
		return 0.f;

	if(ControllerRef) ControllerRef->BBC->SetValueAsBool("Damaged", true);

	FHitResult HitInfo;
	FVector ImpulseDir;
	DamageEvent.GetBestHitInfo(this, DamageCauser, HitInfo, ImpulseDir);

	ACharacterBase* Chr = Cast<ACharacterBase>(HitInfo.GetActor());
	if (!HitInfo.bBlockingHit || Chr == nullptr)
		return 0.f;


	Chr->Health = (HitInfo.BoneName == "head") ? 0.f : Health - Damage;

	if (Chr->Health <= 0.f)
	{
		PlayMontage(DeathMontage, 1.f);
		GetMesh()->bIgnoreRadialForce = true;
		PlayMontage(DeathMontage, 2.f);
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Dead = true;
		StopWeaponFire();
		GetCharacterMovement()->bUseRVOAvoidance = false;

		if (ControllerRef)
		{
			ACoverActor* ReservedCover = Cast<ACoverActor>(ControllerRef->BBC->GetValueAsObject("CoverActor"));
			if (ReservedCover)
			{
				ReservedCover->Available = true;
				ReservedCover->Character = nullptr;
			}
			ControllerRef->GetBrainComponent()->StopLogic("Agent Is Dead");
			ControllerRef->ClearFocus(EAIFocusPriority::LastFocusPriority);
			ControllerRef->GetAIPerceptionComponent()->DestroyComponent(true);
			ControllerRef->AIManager->RemoveAgent(ControllerRef);
			ControllerRef->BBC->SetValueAsBool("Damaged", false);
		}

		return 0.f;
	}

	PlayMontage(HitReactMontage, 1.f);
	return 0.f;
}

void ACharacterBase::SetCollision(bool Newbool)
{
	if (Newbool)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		return;
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

bool ACharacterBase::IsHostile(ACharacterBase* Agent)
{
	return (Faction != Agent->Faction && Agent->Faction != EFaction::Neutral);
}

