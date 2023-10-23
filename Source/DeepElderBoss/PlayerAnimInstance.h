// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEEPELDERBOSS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PerformedAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PerformedRightAttack;

	
};
