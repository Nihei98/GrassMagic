// Fill out your copyright notice in the Description page of Project Settings.

#include "GMResourceAcquirer.h"
#include "GameFramework/PawnMovementComponent.h"

const float UGMResourceAcquirer::Acquire_Tick = 1.0f;
const float UGMResourceAcquirer::Acquire_Delay = 1.0f;

const float UGMResourceAcquirer::Movement_Adjust_Rate = 0.07f;
const float UGMResourceAcquirer::Movement_Adjust_Timer_Interval = 1.0f;
const float UGMResourceAcquirer::Movement_Adjust_Cuttoff = 0.1f;

UGMResourceAcquirer::UGMResourceAcquirer():
	Owner(nullptr),
	MovementOffset(0.0f),
	MaximumMovmentInput(0.0f),
	CurrentState(State::Idle)
{}


void UGMResourceAcquirer::Init(AActor* OwnerActor, float OwnerMovementExpectedInput)
{
	Owner = OwnerActor;
	MaximumMovmentInput = OwnerMovementExpectedInput;

}

void UGMResourceAcquirer::Acquire()
{
	CurrentState = State::Prepare;
}

void UGMResourceAcquirer::StopAcquire()
{
	Owner->GetWorldTimerManager().ClearTimer(TimerHandle_ResourceAcquire);
	CurrentState = State::Idle;
	MovementOffset = 0.0f;
}

bool UGMResourceAcquirer::IsAcquire() const
{
	return CurrentState == State::InProgress;
}

float UGMResourceAcquirer::AdjustMovement(float Value)
{
	switch (CurrentState)
	{
	case State::Idle:
		return Value;
	case State::Prepare:
		return Prepare(Value);
	case State::InProgress:
		return 0.0f;
	default:
		return Value;
	}
}

void UGMResourceAcquirer::OnTickResourceAcquire()
{
	Resources += Resource_Per_Tick;
}

float UGMResourceAcquirer::Prepare(float InputValue)
{
	check(CurrentState == State::Prepare);
	
	APawn* PawnOwner = Cast<APawn>(Owner);
	
	check(PawnOwner);

	float AdjustedValue = 0.0f;

	if (PawnOwner->GetMovementComponent()->GetLastInputVector().IsZero())
		// We stand still, so don't move until we done cast
		MovementOffset = MaximumMovmentInput;
	else if (InputValue != 0.0)
	{
		const float AbsValue = FMath::Abs(InputValue);

		MovementOffset = FMath::FInterpTo(MovementOffset, MaximumMovmentInput, Movement_Adjust_Timer_Interval, Movement_Adjust_Rate);
		AdjustedValue = AbsValue - MovementOffset;

		if (FMath::IsNearlyZero(AdjustedValue, Movement_Adjust_Cuttoff))
		{
			AdjustedValue = 0.0f;
			MovementOffset = MaximumMovmentInput;
		}
	}

	// If we don't move we can cast
	if (MovementOffset >= MaximumMovmentInput)
	{
		CurrentState = State::InProgress;

		// Set timer to start acquire
		Owner->GetWorldTimerManager().SetTimer(TimerHandle_ResourceAcquire, this,
			&UGMResourceAcquirer::OnTickResourceAcquire, Acquire_Tick, true, Acquire_Delay);
	}

	return  InputValue > 0.0f ? AdjustedValue : -AdjustedValue;

}
