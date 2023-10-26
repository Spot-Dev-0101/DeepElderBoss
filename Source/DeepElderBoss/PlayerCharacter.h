// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h" 
#include "Components/BoxComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DEEPELDERBOSS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	UCameraComponent* cam;

	UPROPERTY()
	USceneComponent* armAxisPoint;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* arm;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* AttackCollider;


	virtual void MoveCamera();
	virtual void MouseX(float Value);
	virtual void MouseY(float Value);

	float MouseXValue = 0;
	float MouseYValue = 0;

	UPlayerAnimInstance* PlayerAnim;

	virtual void MoveForward(float Value); 
	virtual void MoveRight(float Value);

	float ForwardValue = 0;
	float RightValue = 0; 

	virtual void Move();

	FVector LastLocation;
	FRotator LastFacingRotation;
	FVector LastMovingDir;

	virtual void MouseAttack();
	virtual void MouseRightAttack();

	float AttackCooldown = 1;
	float AttackTimer = 0;
	bool LeftAttack;
	bool RightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HitEnemy;

};
