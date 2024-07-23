// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#pragma once

#include "MatchmakingStructures.generated.h"

USTRUCT(BlueprintType)
struct EDGEGAPSUBSYSTEM_API FTicketAssignment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Host;

	bool IsValid() const { return !Host.IsEmpty() && Host.Contains(":"); }
	
	static FTicketAssignment FromJson(const TSharedPtr<class FJsonObject>& JsonObject);
};

USTRUCT(BlueprintType)
struct EDGEGAPSUBSYSTEM_API FTicketData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTicketAssignment Assignment;

	bool IsAssigned() const { return Assignment.IsValid(); }
	
	static FTicketData FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};

struct EDGEGAPSUBSYSTEM_API FEdgegapResponse
{
	FString RequestId;
	TSharedPtr<FJsonObject> Data;
	FString Error;

	bool HasError() const { return !Error.IsEmpty(); }
	
	template<typename TData>
	TData GetData() const { return TData::FromJson(Data); }
	
	static FEdgegapResponse FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static FEdgegapResponse FromJson(const FString& JsonString);

};

USTRUCT(BlueprintType, Blueprintable)
struct EDGEGAPSUBSYSTEM_API FMatchmakerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ProfileId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> Selectors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> Filters;

	TSharedPtr<FJsonObject> ToJson() const;
	FString ToJsonString() const;
};

USTRUCT(BlueprintType)
struct EDGEGAPSUBSYSTEM_API FMatchmakerConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="Internal ID for reference"))
	FString Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="URL"))
	FString Url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="API Token"))
	FString Token;
	
	bool IsValid() const;
};