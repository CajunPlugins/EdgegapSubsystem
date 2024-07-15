// Copyright 2024 Cajun Pro LLC. All Rights Reserved.
// ReSharper disable CppUEBlueprintImplementableEventNotImplemented
// ReSharper disable CppUEBlueprintCallableFunctionUnused

#pragma once

#include "MatchmakingStructures.h"
#include "GameFramework/Actor.h"
#include "Matchmaker.generated.h"

UCLASS(BlueprintType)
class EDGEGAPSUBSYSTEM_API AMatchmaker : public AActor
{
	GENERATED_BODY()

public:
	AMatchmaker();

	UPROPERTY(EditDefaultsOnly)
	FMatchmakerConfig Config;
	
	UFUNCTION(BlueprintCallable, Category="Matchmaking")
	void StartMatchmaking();

	UFUNCTION(BlueprintCallable, Category="Matchmaking")
	void CancelMatchmaking();

	UFUNCTION(BlueprintCallable, Category="Matchmaking")
	void SetSettings(const FMatchmakerSettings& NewSettings);

	UFUNCTION(BlueprintCallable, Category="Matchmaking")
	void SetProfileId(const FString& NewProfileId);
	
	UFUNCTION(BlueprintCallable, Category="Matchmaking")
	void SetFilters(const TMap<FString, FString>& NewFilters);

	UFUNCTION(BlueprintCallable, Category="Matchmaking")
	void SetSelectors(const TMap<FString, FString>& NewSelectors);

	UFUNCTION(BlueprintCallable, Category="Matchmaking", meta=(AutoCreateRefTerm="UrlOptions"))
	void ConnectToMatch(const FString& UrlOptions);
	
	UFUNCTION(BlueprintPure, Category="Matchmaking")
	bool IsMatchmaking() const;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Matchmaking")
	void OnProgress();

	UFUNCTION(BlueprintImplementableEvent, Category="Matchmaking")
	void OnMatchFound(const FString& Host, const FString& Ticket);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Matchmaking")
	void OnCancelled();

	UFUNCTION(BlueprintImplementableEvent, Category="Matchmaking")
	void OnError(const FString& Error);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter="SetSettings", Setter="SetSettings")
	FMatchmakerSettings Settings;
	
	UFUNCTION()
	virtual void HandleTicketCreated(FTicketData TicketData);
	UFUNCTION()
	virtual void HandleTicketRetrieved(FTicketData TicketData);
	UFUNCTION()
	virtual void HandleTicketDeleted(FTicketData TicketData);
	UFUNCTION()
	virtual void HandleMatchFound();
	UFUNCTION()
	virtual void HandleError(FString Reason);

	UFUNCTION()
	void PollCurrentProgress();
	
private:
	UPROPERTY(Transient)
	bool bMatchmaking = false;

	UPROPERTY(Transient)
	FTicketData CurrentTicketData;

	UPROPERTY(Transient)
	FTimerHandle MatchStatusTimerHandle;
};
