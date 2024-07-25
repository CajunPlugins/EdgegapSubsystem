// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#pragma once

#include "Interfaces/IHttpRequest.h"
#include "Matchmaking/MatchmakingStructures.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EdgegapSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTicketResponseDelegate, FTicketData, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResponseDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorDeletage, FString, Reason = "");

DECLARE_LOG_CATEGORY_EXTERN(LogEdgegapSubsystem, Log, All);

#define EGSS_LOG(Verbosity, Format, ...) UE_LOG(LogEdgegapSubsystem, Verbosity, Format, ##__VA_ARGS__)

/**
 * 
 */
UCLASS()
class EDGEGAPSUBSYSTEM_API UEdgegapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Subsystems|Edgegap|Matchmaking")
	void CreateTicket(const FMatchmakerConfig& Config, const FMatchmakerSettings& Settings);

	UFUNCTION(BlueprintCallable, Category="Subsystems|Edgegap|Matchmaking")
	void GetTicket(const FMatchmakerConfig& Config, const FString& TicketId);

	UFUNCTION(BlueprintCallable, Category="Subsystems|Edgegap|Matchmaking")
	void DeleteTicket(const FMatchmakerConfig& Config, const FString& TicketId);

	UFUNCTION(BlueprintPure, Category="Subsystems|Edgegap|Matchmaking", meta=(AutoCreateRefTerm="Id"))
	class AMatchmaker* GetMatchmaker(const FString& Id);

	void RegisterMatchmaker(const FString& Id, class AMatchmaker* Matchmaker);
	void UnregisterMatchmaker(const FString& Id);
protected:
	void HandleCreateTicket(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool bWasSuccessful);
	void HandleGetTicket(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void HandleDeleteTicket(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnTicketResponseDelegate OnTicketCreated;
	
	UPROPERTY(BlueprintAssignable)
	FOnTicketResponseDelegate OnTicketRetrieved;

	UPROPERTY(BlueprintAssignable)
	FOnResponseDelegate OnTicketDeleted;

	UPROPERTY(BlueprintAssignable)
	FOnErrorDeletage OnError;
	
protected:
	UPROPERTY()
	TMap<FString, class AMatchmaker*> RegisteredMatchmakers;
	
private:
	static TSharedRef<IHttpRequest> CreateRequest(const FMatchmakerConfig& Config, const FString& Endpoint, const FString& Method = "GET");

public:
	static UEdgegapSubsystem* Get(const UObject* WorldContextObject); 
};
