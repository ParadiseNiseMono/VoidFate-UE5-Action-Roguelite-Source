// Paradise NiseMono All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerStateDataRegistry.generated.h"

class UListDataObject_Collection;
class UListDataObject_Base;
/**
 * 
 */
UCLASS()
class VOIDFATE_API UPlayerStateDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	//Gets called by player state screen right after the object of type UOptionsDataRegistry is created.
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }

	TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

private:
	void FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;
	
	void InitRewardsCollectionTab(ULocalPlayer* InOwningLocalPlayer);
	
	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;
};
