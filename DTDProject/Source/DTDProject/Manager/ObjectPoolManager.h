#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "ObjectPoolManager.generated.h"

USTRUCT()
struct FObjectPoolArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Pool;
};

UCLASS()
class DTDPROJECT_API UObjectPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* SpawnObject(TSubclassOf<AActor> ObjectClass, const FVector& Position, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, Category ="Object Pool")
	void ReturnObject(AActor* ObjectToReturn);

private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FObjectPoolArray> ObjectPools;

	virtual UWorld* GetWorld() const override;
};
