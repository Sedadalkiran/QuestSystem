// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQuestInfo.h"
#include "GameplayTagAssetInterface.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestComponent : public UActorComponent, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool CanTakeQuest(FGameplayTag QuestID);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ActiveTaskIndex = -1;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGameplayTag CurrentQuest;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> FinishedQuests;

	//Key: QuestID Value:Task Array
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FTaskData> CurrentQuestTasks;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> QuestDataTable;

	UFUNCTION(BlueprintCallable)
	bool TakeNewQuest(FGameplayTag QuestID);

	UFUNCTION(BlueprintCallable)
	EQuestRefreshState RefreshQuest();

	UFUNCTION(BlueprintCallable)
	void UpdateQuestList();


	UFUNCTION(BlueprintCallable)
	bool CompleteQuest();

	UFUNCTION(BlueprintCallable)
	TArray<FTaskRequirement> GetCurrentTaskRequirements(FGameplayTag TaskID);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGameplayTagContainer CompletedActions;

	UFUNCTION(BlueprintCallable)
	bool IsRequiredQuestsCompletedForQuest(FGameplayTag QuestID);

	UFUNCTION(BlueprintCallable)
	bool AddCompletedActionTag(FGameplayTag CompletedActionTag);

	//START IGameplayTagAssetInterface

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;

	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	//END IGameplayTagAssetInterface
};
