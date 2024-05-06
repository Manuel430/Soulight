// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayerCharacter.h"

#include "Camera/CameraComponent.h"

#include "Player/SPlayerController.h"

#include "Framework/SGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/SceneCaptureComponent2D.h"

ASPlayerCharacter::ASPlayerCharacter()
{
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	viewCamera = CreateDefaultSubobject<UCameraComponent>("View Camera");

	miniMapBoom = CreateDefaultSubobject<USpringArmComponent>("Mini Map Boom");
	miniMapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Mini Map Camera");

	cameraBoom->SetupAttachment(GetRootComponent());
	viewCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);

	miniMapBoom->SetupAttachment(GetRootComponent());
	miniMapCapture->SetupAttachment(miniMapBoom, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1080.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
}

void ASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController) {
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		InputSubsystem->ClearAllMappings();
		InputSubsystem->AddMappingContext(InputMapping, 0);
	}


	UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComp)
	{
		enhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASPlayerCharacter::Move);
		enhancedInputComp->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ASPlayerCharacter::Aim);
		enhancedInputComp->BindAction(LookInputAction, ETriggerEvent::Completed, this, &ASPlayerCharacter::AimComplete);
		enhancedInputComp->BindAction(ZoomInputAction, ETriggerEvent::Triggered, this, &ASPlayerCharacter::Zoom);
		enhancedInputComp->BindAction(RunInputAction, ETriggerEvent::Started, this, &ASPlayerCharacter::StartRunning);
		enhancedInputComp->BindAction(RunInputAction, ETriggerEvent::Completed, this, &ASPlayerCharacter::StoppedRunning);
	}
}

void ASPlayerCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D Input = InputValue.Get<FVector2D>();
	Input.Normalize();

	AddMovementInput(Input.Y * GetMoveFwdDir() + Input.X * GetMoveRightDir());
}

void ASPlayerCharacter::Zoom(const FInputActionValue& InputValue)
{
	float Zoom = InputValue.Get<float>();

	if (cameraBoom) 
	{
		cameraBoom->TargetArmLength = FMath::Clamp
		(
			cameraBoom->TargetArmLength + (Zoom * 100.0f),
			7000,	// Min Target Distance
			10000	// Max Target Distance
		);
	}
}

void ASPlayerCharacter::Aim(const FInputActionValue& InputValue)
{
	FVector2D Input = InputValue.Get<FVector2D>();

	GetCharacterMovement()->bOrientRotationToMovement = false;

	Input.Normalize();
	FRotator NewRotation = FRotator(0.f, FMath::RadiansToDegrees(FMath::Atan2(Input.Y, Input.X)) * Sensitivity, 0.f);

	SetActorRotation(NewRotation);

}

void ASPlayerCharacter::AimComplete()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASPlayerCharacter::StartRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ASPlayerCharacter::StoppedRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

FVector ASPlayerCharacter::GetMoveFwdDir() const
{
	FVector CamerFwd = viewCamera->GetForwardVector();
	CamerFwd.Z = 0;
	return CamerFwd.GetSafeNormal();
}

FVector ASPlayerCharacter::GetMoveRightDir() const
{
	return viewCamera->GetRightVector();
}
