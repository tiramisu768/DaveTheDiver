// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundData.h"
#include "Engine/DataTable.h"
#include "SoundManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class DTDPROJECT_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	UAudioComponent* PlaySFX(ESoundKey SoundKey);

	UFUNCTION(BlueprintCallable)
	void StopSFX(UAudioComponent* AudioComponent);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBGM(ESoundKey SoundKey, bool bLoop = true);
	
	UFUNCTION(BlueprintCallable)
	void StopBGM(); 
	
	UFUNCTION(BlueprintCallable)
	void FadeBGM(float TargetVolume, float Duration); 
	
	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float InVolume);

	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float InVolume);

	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float InVolume);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> SoundDataTable;

	UPROPERTY(Config)
	FSoftObjectPath SoundDataTablePath;

	UPROPERTY()
	UAudioComponent* MusicComponent = nullptr;

	float MasterVolume = 1.0f;
	float SFXVolume = 1.0f;
	float MusicVolume = 1.0f;

	float CurrentBGMVolume = 0.0f;
	bool bBGMShouldLoop = false;

	FTimerHandle FadeTimerHandle;
	float FadeStart = 0.f;
	float FadeTarget = 0.f;
	float FadeDuration = 0.f;
	float FadeElapsed = 0.f;

	FName GetSoundKeyAsName(ESoundKey SoundKey) const;
	UAudioComponent* PlaySFX_Internal(USoundBase* Sound, float Volume = 1.0f);
	void PlayBGM_Internal(USoundBase* Music, float Volume = 1.0f, bool bLoop = true);
	void EnsureMusicComponent();
	void UpdateFadeTick();


	UFUNCTION()
	void HandleMusicFinished();
};
