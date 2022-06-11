// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle              UMETA(DisplayName = "Idle"),
	ManDown           UMETA(DisplayName = "Man Down"),
	Investigate       UMETA(DisplayName = "Investigate"),
	LostEnemy         UMETA(DisplayName = "Lost Enemy"),
	Search            UMETA(DisplayName = "Search"),
	Attack            UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Enemy            UMETA(DisplayName = "Enemy"),
	Friendly         UMETA(DisplayName = "Friendly"),
	Neutral          UMETA(DisplayName = "Neutral")
};

UENUM(BlueprintType)
enum class ECombatRole : uint8
{
	Engager           UMETA(DisplayName = "Engager"),
	Defender          UMETA(DisplayName = "Defender"),
	Sniper            UMETA(DisplayName = "Sniper")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	SoftCover         UMETA(DisplayName = "Soft Cover"),
	ApproachingCover  UMETA(DisplayName = "Approaching Cover"),
	Flank             UMETA(DisplayName = "Flank"),
	HoldCover         UMETA(DisplayName = "HoldCover"),
	RetreatingCover   UMETA(DisplayName = "RetreatingCover"),
	Evade             UMETA(DisplayName = "Evade")
};