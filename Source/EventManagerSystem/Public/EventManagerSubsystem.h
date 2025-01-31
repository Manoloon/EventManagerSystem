// Copyright Pablo Rodrigo Sanchez, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EventManagerSubsystem.generated.h"

/**
 * Event manager Subsystem that handle all events sent by the EventListenerActor and the EventListenerComponent.
 * Events "CHK_" are for set checkpoints
 * Events "RST_" are sent to reset the state of the actors that are listening to this.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventCalled,FName);

namespace EventManagerLabels
{
	static const FString CheckpointPrefix = TEXT("CHK_");
	static const FString ResetEventPrefix = TEXT("RST_");
}
UCLASS(meta = (DisableNativeTick))
class EVENTMANAGERSYSTEM_API UEventManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	float ResetTime = 2.f;
	UPROPERTY(Replicated)
	FName LastCalledEventName = NAME_None;
	FName LastCheckpointEventName = NAME_None;

	FTimerHandle ResetTimerHandle;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CallEvent(FName Event);

	//UFUNCTION(BlueprintImplementableEvent)
	//void K2_OnCallEvent(FName Event);
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	FOnEventCalled OnEventCalledDelegate;
	UFUNCTION(BlueprintCallable)
	void CallEvent(FName Event);
	UFUNCTION(BlueprintCallable)
	void ResetLastCheckpoint(float TimeToReset);
};