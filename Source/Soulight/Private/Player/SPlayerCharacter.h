// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SCharacterBase.h"
#include "SPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USceneCaptureComponent2D;
/**
 * 
 */
UCLASS()
class ASPlayerCharacter : public ASCharacterBase
{
	GENERATED_BODY()
	
public:
	ASPlayerCharacter();


private:
	UPROPERTY(visibleAnywhere, Category = "View")
		USpringArmComponent* cameraBoom;
	UPROPERTY(visibleAnywhere, Category = "View")
		UCameraComponent* viewCamera;

	UPROPERTY(visibleAnywhere, Category = "MiniMap")
		USpringArmComponent* MiniMapSpringArm;

	UPROPERTY(visibleAnywhere, Category = "MiniMap")
		USceneCaptureComponent2D* MiniMapCapture;


/*****************************************************/
/*                       Input                       */
/*****************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* MoveInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* RunInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* LookInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* ZoomInputAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void Move(const FInputActionValue& InputValue);
	UFUNCTION()
		void Zoom(const FInputActionValue& InputValue);
	UFUNCTION()
		void Look(const FInputActionValue& InputValue);
	UFUNCTION()
		void StartRunning();
	UFUNCTION()
		void StoppedRunning();


	FVector GetMoveFwdDir() const;
	FVector GetMoveRightDir() const;


};
