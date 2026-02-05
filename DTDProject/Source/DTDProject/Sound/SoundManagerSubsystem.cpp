// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/SoundManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "TimerManager.h"
#include "Engine/World.h"

void USoundManagerSubsystem::PlaySFX(ESoundKey SoundKey)
{
	if (!SoundDataTable || SoundKey==ESoundKey::None) return;

	const FName SoundName = GetSoundKeyAsName(SoundKey);
	if (SoundName.IsNone()) return;

	const FString ContextString = TEXT("SoundManagerSubsystem::PlaySFX");
	if (FSoundData* Row = SoundDataTable->FindRow<FSoundData>(SoundName, ContextString))
	{
		if (USoundBase* SoundToPlay = Row->Sound.LoadSynchronous())
		{
			PlaySFX_Internal(SoundToPlay, Row->DefaultVolume);
		}
	}
}

void USoundManagerSubsystem::PlayBGM(ESoundKey SoundKey, bool bLoop)
{
	if (!SoundDataTable || SoundKey == ESoundKey::None) return;

	const FName SoundName = GetSoundKeyAsName(SoundKey);
	if (SoundName.IsNone()) return;

	const FString ContextString = TEXT("SoundManagerSubsystem::PlayBGM");
	if (FSoundData* Row = SoundDataTable->FindRow<FSoundData>(SoundName, ContextString))
	{
		if (USoundBase* MusicToPlay = Row->Sound.LoadSynchronous())
		{
			PlayBGM_Internal(MusicToPlay, Row->DefaultVolume, bLoop);
		}
	}
}

void USoundManagerSubsystem::PlaySFX_Internal(USoundBase* Sound, float Volume)
{
	if (!Sound) return;
	UWorld* World = GetWorld();
	if (!World) return;

	const float FinalVol = MasterVolume * SFXVolume * Volume;
	UGameplayStatics::PlaySound2D(World, Sound, FinalVol);
}

void USoundManagerSubsystem::PlayBGM_Internal(USoundBase* Music, float Volume, bool bLoop)
{
	if (!Music) return;
	EnsureMusicComponent();
	if (!MusicComponent) return;

	MusicComponent->OnAudioFinished.Clear();

	MusicComponent->SetSound(Music);
	bBGMShouldLoop = bLoop;

	const float NewVol = MasterVolume * MusicVolume * Volume;
	MusicComponent->SetVolumeMultiplier(NewVol);

	CurrentBGMVolume = NewVol;

	if (bBGMShouldLoop)
	{
		MusicComponent->OnAudioFinished.AddDynamic(this, &USoundManagerSubsystem::HandleMusicFinished);
	}
	MusicComponent->Play();
}

void USoundManagerSubsystem::StopBGM()
{
	if (MusicComponent && MusicComponent->IsPlaying())
	{
		MusicComponent->Stop();
	}
}

void USoundManagerSubsystem::HandleMusicFinished()
{
	if (MusicComponent && bBGMShouldLoop)
	{
		MusicComponent->Play();
	}
}


void USoundManagerSubsystem::FadeBGM(float TargetVolume, float Duration)
{
	EnsureMusicComponent();
	if (!MusicComponent)return;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}

	FadeStart = CurrentBGMVolume;
	FadeTarget = FMath::Clamp(TargetVolume * MasterVolume * MusicVolume, 0.f, 5.f);
	FadeDuration = FMath::Max(Duration, KINDA_SMALL_NUMBER);
	FadeElapsed = 0.f;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(FadeTimerHandle, [this]() {UpdateFadeTick(); },
			0.02f,
			true
		);
	}
}

void USoundManagerSubsystem::SetMasterVolume(float InVolume)
{
	MasterVolume = FMath::Clamp(InVolume, 0.f, 5.f);
	if (MusicComponent)
	{
		MusicComponent->SetVolumeMultiplier(MasterVolume * MusicVolume);
	}
}

void USoundManagerSubsystem::SetSFXVolume(float InVolume)
{
	SFXVolume = FMath::Clamp(InVolume, 0.f, 5.f);
}

void USoundManagerSubsystem::SetMusicVolume(float InVolume)
{
	MusicVolume = FMath::Clamp(InVolume, 0.f, 5.f);
	if (MusicComponent)
	{
		MusicComponent->SetVolumeMultiplier(MasterVolume * MusicVolume);
	}
}

void USoundManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	MusicComponent = nullptr;
	MasterVolume = 1.0f;
	SFXVolume = 1.0f;
	MusicVolume = 1.0f;

	if (SoundDataTablePath.IsValid())
	{
		SoundDataTable = Cast<UDataTable>(SoundDataTablePath.TryLoad());
	}
}

void USoundManagerSubsystem::Deinitialize()
{
	if (MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent->UnregisterComponent();
		MusicComponent = nullptr;
	}
	Super::Deinitialize();
}

FName USoundManagerSubsystem::GetSoundKeyAsName(ESoundKey SoundKey) const
{
	const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/DTDProject.ESoundKey"), true);
	if (!EnumPtr)
	{
		return NAME_None;
	}
	return FName(*EnumPtr->GetNameStringByValue(static_cast<int64>(SoundKey)));
}

void USoundManagerSubsystem::EnsureMusicComponent()
{
	if (MusicComponent) return;
	UWorld* World = GetWorld();
	if (!World) return;

	MusicComponent = NewObject<UAudioComponent>(GetGameInstance());
	if (MusicComponent)
	{
		MusicComponent->bAutoActivate = false;
		MusicComponent->bIsUISound = false;
		MusicComponent->RegisterComponentWithWorld(World);
	}
}

void USoundManagerSubsystem::UpdateFadeTick()
{
	if (!MusicComponent || !GetWorld()) return;

	FadeElapsed += 0.02f;
	const float Alpha = FMath::Clamp(FadeElapsed / FadeDuration, 0.f, 1.f);
	const float NewVol = FMath::Lerp(FadeStart, FadeTarget, Alpha);
	MusicComponent->SetVolumeMultiplier(NewVol);

	CurrentBGMVolume = NewVol;

	if (Alpha >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}
}