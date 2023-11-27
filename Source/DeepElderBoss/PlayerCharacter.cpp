// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT(text), fstring))

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//Create Camera arm
	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	//Create Axis point for the camera are to enable horizontal movement
	armAxisPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpringAxisPoint"));

	//armAxisPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	armAxisPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Pelvis"));

	armAxisPoint->SetWorldRotation(FRotator(0, 90, 0));

	

	arm->AttachToComponent(armAxisPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	arm->TargetArmLength = 300.f;

	arm->bEnableCameraLag = true;
	arm->CameraLagSpeed = 8.0f;

	arm->SetRelativeLocation(FVector(0, 0, 75));

	cam->AttachToComponent(arm, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);

	AttackCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	AttackCollider->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Mace_head"));


}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MouseX"), this, &APlayerCharacter::MouseX);
	PlayerInputComponent->BindAxis(TEXT("MouseY"), this, &APlayerCharacter::MouseY);

	PlayerInputComponent->BindAxis(TEXT("Move_Forward"), this, &APlayerCharacter::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("Move_Backward"), this, &APlayerCharacter::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("Move_Left"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Move_Right"), this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("MouseRight"), this, &APlayerCharacter::MouseRight);

	PlayerInputComponent->BindAction(TEXT("MouseAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::MouseAttack);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	print("Start");

	PlayerAnim = (UPlayerAnimInstance*)GetMesh()->GetAnimInstance();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove == true) {
		Move();
	}
	
	MoveCamera();

	//PlayerAnim->Velocity = GetVelocity();

	AttackTimer -= DeltaTime;

	if (AttackTimer > 0) {
		if (LeftAttack == true) {
			PlayerAnim->Attack = true;
		}
		if (RightAttack == true) {
			PlayerAnim->RightAttack = true;
		}
	}
	else {
		if (LeftAttack == true) {
			PlayerAnim->Attack = false;
			LeftAttack = false;
		}
		if (RightAttack == true) {
			PlayerAnim->RightAttack = false;
			RightAttack = false;
		}
	}

	if (PlayerAnim->PerformedAttack == true) {
		print("Perform Attack");

		print(FString::SanitizeFloat(FVector::Dist(GetActorLocation(), EnemyActor->GetActorLocation())));

		if (PlayerAnim->Hover == true) {
			if (FVector::Dist(GetActorLocation(), EnemyActor->GetActorLocation()) < 1000) {
				print("Enemy Close Enough");
				HitEnemy = true;
				print("Hit Enemy");
			}
		}
		else {

			TArray<AActor*> Result;

			AttackCollider->GetOverlappingActors(Result);


			for (AActor* OverlappedActor : Result) {
				if (OverlappedActor->GetName().Contains("BP_Enemy")) {
					HitEnemy = true;
					print("Hit Enemy");
				}

			}
		}

		PlayerAnim->PerformedAttack = false;
	}

	if (PlayerAnim->PerformedRightAttack == true) {
		print("Perform Right Attack");
		PlayerAnim->PerformedRightAttack = false;
	}

	if (PlacedFootPrintThisFrame == true) {
		PlacedFootPrintThisFrame = false;
	}
	
}

void APlayerCharacter::Move()
{

	FVector ForwardMoveDirection = FRotationMatrix(armAxisPoint->GetComponentRotation()).GetScaledAxis(EAxis::X);
	FVector RightMoveDirection = FRotationMatrix(armAxisPoint->GetComponentRotation()).GetScaledAxis(EAxis::Y);

	FVector CurrentVelocity = GetVelocity();
	CurrentVelocity.Normalize();
	

	FVector NewAnimValue = FVector(((MouseXValue / 4) * 100), GetVelocity().Length() * ForwardValue, 0);

	


	if (RightValue == 1) {
		NewAnimValue.X = 150;
	} else if (RightValue == -1) {
		NewAnimValue.X = -150;
	}

	PlayerAnim->Velocity = FMath::Lerp(PlayerAnim->Velocity, NewAnimValue, 0.1);


	//print(PlayerAnim->Velocity.ToString());

	AddMovementInput(ForwardMoveDirection, ForwardValue * 1);
	AddMovementInput(RightMoveDirection, RightValue * 1);

	FVector MovingDir = ((LastLocation - GetActorLocation()));

	if (MovingDir.Length() == 0) {
		MovingDir = LastMovingDir;
	}

	FRotator MovingRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), (-(MovingDir * 25) + GetActorLocation()));

	FRotator FacingAndMovingRotationDiff = MovingRotation - LastFacingRotation;
	FQuat FacingRotationQuat = FQuat::Slerp(FQuat(LastFacingRotation), FQuat(MovingRotation), 0.1);
	FRotator FacingRotation = FacingRotationQuat.Rotator();
	FRotator NewFacingRotator = FRotator(0, FacingRotation.Yaw - 90, 0);

	//GetMesh()->SetWorldRotation(NewFacingRotator);
	FRotator NewRotation = (-RightMoveDirection).Rotation();
	FRotator NewRotationLerped = FMath::Lerp(GetMesh()->GetComponentRotation(), NewRotation, 0.1);

	if (PlayerAnim->HoverHold == true) {
		//print(PlayerAnim->Velocity.ToString());
		NewRotationLerped.Roll = PlayerAnim->Velocity.Y/100;
		NewRotationLerped.Pitch = PlayerAnim->Velocity.X / 25;
		//print(NewRotationLerped.ToString());
	}

	GetMesh()->SetWorldRotation(NewRotationLerped);

	LastLocation = GetActorLocation();
	LastFacingRotation = FacingRotation;
	LastMovingDir = MovingDir;

	


}

void APlayerCharacter::MouseAttack()
{
	print("Attack Left");
	if (AttackTimer <= 0) {
		LeftAttack = true;
		if (PlayerAnim->Hover == true) {
			AttackTimer = HoverAttackCooldown;
		}
		else {
			AttackTimer = AttackCooldown;
		}
	}
	
	
}

void APlayerCharacter::MouseRight(float value)
{
	if (value > 0) {
		PlayerAnim->HoverHold = true;
		PlayerAnim->Hover = true;
		FVector EndLocation = GetActorLocation() + FVector(0, 0, HoverHeight) + FVector(0, 0, -90);
		FVector NewLocation = FMath::Lerp(GetMesh()->GetComponentLocation(), EndLocation, 0.03);
		GetMesh()->SetWorldLocation(NewLocation);

		if (FVector::Dist(GetMesh()->GetComponentLocation(), EndLocation) < 10) {
			//print("At Top");
			//Disable attacking until at the top
		}


		float TargetCameraOffset = CameraOffsetHover;
		float NewCameraOffset = FMath::Lerp(CameraOffset, TargetCameraOffset, 0.03);
		CameraOffset = NewCameraOffset;
	}
	else {
		PlayerAnim->HoverHold = false;
		FVector EndLocation = GetActorLocation() + FVector(0, 0, -90);
		FVector NewLocation = FMath::Lerp(GetMesh()->GetComponentLocation(), EndLocation, 0.075);
		GetMesh()->SetWorldLocation(NewLocation);


		float TargetCameraOffset = CameraOffsetGround;
		float NewCameraOffset = FMath::Lerp(CameraOffset, TargetCameraOffset, 0.075);
		CameraOffset = NewCameraOffset;
	}
}

void APlayerCharacter::LeftPlant()
{
	if(PlacedFootPrintThisFrame == false) {
		print("LeftPlant");
		SpawnFootPrintDecal("Foot_L");
	}
}

void APlayerCharacter::RightPlant()
{
	if (PlacedFootPrintThisFrame == false) {
		print("RightPlant");
		SpawnFootPrintDecal("Foot_R");
	}
	
}

void APlayerCharacter::SpawnFootPrintDecal(FName SocketName)
{

	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(GetMesh()->GetSocketLocation(SocketName), FRotator());
	if (decal)
	{
		decal->SetDecalMaterial(FootStepDecalMaterial);
		//decal->SetActorRotation(GetMesh()->GetComponentRotation() + FRotator(-90, -75, 0));
		decal->SetActorRotation(GetMesh()->GetSocketRotation(SocketName) + FRotator(-90, -160, 0));
		decal->SetActorScale3D(FVector(0.05, 0.05, 0.05));
		
		//decal->SetLifeSpan(2.0f);
		PlacedFootPrintThisFrame = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
	}

}


void APlayerCharacter::MoveCamera()
{

	float tempY = arm->GetRelativeRotation().Pitch + MouseYValue;
	if (tempY < 25 && tempY > -85) {
		arm->AddLocalRotation(FRotator(MouseYValue, 0, 0));
		
	}

	//Rotate the arm axis
	float tempX = armAxisPoint->GetRelativeRotation().Yaw + MouseXValue;
	armAxisPoint->AddLocalRotation(FRotator(0, MouseXValue, 0));
	//AddActorWorldRotation(FRotator(0, MouseXValue, 0));

	armAxisPoint->SetWorldLocation(GetMesh()->GetComponentLocation() + FVector(0, 0, 80));

}

void APlayerCharacter::MouseX(float Value) {
	MouseXValue = Value;
	//print("Y: " + FString::SanitizeFloat(Value));
}

void APlayerCharacter::MouseY(float Value)
{
	MouseYValue = Value;
	//print("Y: " + FString::SanitizeFloat(Value));
}

void APlayerCharacter::MoveForward(float Value)
{
	ForwardValue = Value;
	//print("F: " + FString::SanitizeFloat(Value));
}


void APlayerCharacter::MoveRight(float Value)
{
	RightValue = Value;
	//print("R: " + FString::SanitizeFloat(Value));
}



