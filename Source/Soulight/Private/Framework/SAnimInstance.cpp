// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerMovemmentComp = OwnerCharacter->GetCharacterMovement();
	}
}

void USAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter && OwnerMovemmentComp)
	{
		Speed = OwnerCharacter->GetVelocity().Length();
	}
}
