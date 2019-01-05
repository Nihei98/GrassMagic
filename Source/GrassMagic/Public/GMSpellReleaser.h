// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GMSpellReleaser.generated.h"

/**
 * 
 */
UCLASS()
class GRASSMAGIC_API UGMSpellReleaser : public UObject
{
	GENERATED_BODY()

	const static float Focus_Min;
	const static float Focus_Tick_Interva; 
	const static float Focus_Tick_Increment;
	const static float Focus_Tick_Decrement;

public:

	UGMSpellReleaser();
	~UGMSpellReleaser();

	void Init(AActor* OwnerActor);

	void StartRelease();
	void StopRelease();
	
	float GetFocus() const noexcept { return Focus; }

private:

	UFUNCTION()
	void OnTickSpellRelease();

	FTimerHandle TimerHandler_Release;

	float Focus;

	TWeakObjectPtr<AActor> Owner;

	bool IsReleasingCurrently;
	
};