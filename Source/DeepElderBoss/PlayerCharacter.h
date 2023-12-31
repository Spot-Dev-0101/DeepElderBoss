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
#include "Engine/DecalActor.h" 
#include "Materials/MaterialInstance.h" 
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

	UPROPERTY(VisibleAnywhere, Category="1. Player Settings")
	USpringArmComponent* arm;

	UPROPERTY(VisibleAnywhere, Category = "1. Player Settings")
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
	virtual void MouseRight(float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Player Settings")
	float AttackCooldown = 0.3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Player Settings")
	float HoverAttackCooldown = 2;


	float AttackTimer = 0;
	bool LeftAttack;
	bool RightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Player Settings")
	float HoverHeight = 160;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Player Settings")
	float HoverMovementMax = 6;

	float CameraOffsetGround = 90;
	float CameraOffsetHover = 60;

	float CameraOffset = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Player Settings")
	bool HitEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Player Settings")
	UMaterialInstance* FootStepDecalMaterial;

	UFUNCTION(BlueprintCallable)
	void LeftPlant();

	UFUNCTION(BlueprintCallable)
	void RightPlant();

	void SpawnFootPrintDecal(FName SocketName);

	bool PlacedFootPrintThisFrame = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Player Settings")
	bool CanMove = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Player Settings")
	AActor* EnemyActor;
};
