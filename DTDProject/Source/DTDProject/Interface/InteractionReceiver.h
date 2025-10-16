// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DTDPROJECT_API IInteractionReceiver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnBeginInteractable(class IInteractionObject* Object) = 0;
};
