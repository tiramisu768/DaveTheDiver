#include "Manager/ObjectPoolManager.h"
#include "Interface/PoolableObject.h"

AActor* UObjectPoolManager::SpawnObject(TSubclassOf<AActor> ObjectClass, const FVector& Position, const FRotator& Rotation)
{
	if (!ObjectClass)
	{
		return nullptr;
	}

	FObjectPoolArray& PoolWrapper = ObjectPools.FindOrAdd(ObjectClass);
	TArray<TObjectPtr<AActor>>& Pool = PoolWrapper.Pool;
	AActor* SpawnedObject = nullptr;

	for (AActor* PooledObject : Pool)
	{
		if (PooledObject)
		{
			IPoolableObject* Poolable = Cast<IPoolableObject>(PooledObject);
			if (Poolable && Poolable->IsReadyToReturn())
			{
				SpawnedObject = PooledObject;
				break;
			}
		}
	}

	if (SpawnedObject)
	{
		IPoolableObject* Poolable = Cast<IPoolableObject>(SpawnedObject);
		if (Poolable)
		{
			Poolable->OnSpawnFromPool(Position, Rotation);
		}
	}
	else
	{
		UWorld* World = GetWorld();
		if(World)
		{
			SpawnedObject = World->SpawnActor<AActor>(ObjectClass, Position, Rotation, FActorSpawnParameters());
			if (SpawnedObject)
			{
				IPoolableObject* Poolable = Cast<IPoolableObject>(SpawnedObject);
				if (Poolable)
				{
					Poolable->OnSpawnFromPool(Position, Rotation);
					Pool.Add(SpawnedObject);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, FString::Printf(TEXT("%s does not implement IPoolableObject "), *SpawnedObject->GetName()));
				}
			}
		}
	}

	return SpawnedObject;
}

void UObjectPoolManager::ReturnObject(AActor* ObjectToReturn)
{
	if (ObjectToReturn)
	{
		IPoolableObject* Poolable = Cast<IPoolableObject>(ObjectToReturn);
		if (Poolable)
		{
			Poolable->OnReturnToPool();
		}
		else
		{
			ObjectToReturn->Destroy();
		}
	}
}

UWorld* UObjectPoolManager::GetWorld() const
{
	return GetGameInstance()->GetWorld();
}
