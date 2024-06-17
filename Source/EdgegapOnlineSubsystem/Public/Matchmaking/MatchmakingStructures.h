#pragma once

#include "MatchmakingStructures.generated.h"

USTRUCT(BlueprintType)
struct EDGEGAPONLINESUBSYSTEM_API FTicketAssignment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Host;

	bool IsValid() const { return !Host.IsEmpty() && Host.Contains(":"); }
	
	static FTicketAssignment FromJson(const TSharedPtr<class FJsonObject>& JsonObject);
};

USTRUCT(BlueprintType)
struct EDGEGAPONLINESUBSYSTEM_API FTicketData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTicketAssignment Assignment;

	bool IsAssigned() const { return Assignment.IsValid(); }
	
	static FTicketData FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};

struct EDGEGAPONLINESUBSYSTEM_API FEdgegapResponse
{
	FString RequestId;
	TSharedPtr<FJsonObject> Data;

	template<typename TData>
	TData GetData() const { return TData::FromJson(Data); }
	
	static FEdgegapResponse FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static FEdgegapResponse FromJson(const FString& JsonString);

};

USTRUCT(BlueprintType, Blueprintable)
struct EDGEGAPONLINESUBSYSTEM_API FMatchmakerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ProfileId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> Selectors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> Filters;

	TSharedPtr<FJsonObject> ToJson() const;
	FString ToJsonString() const;
};

USTRUCT(BlueprintType)
struct EDGEGAPONLINESUBSYSTEM_API FMatchmakerConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip="Internal ID for reference"))
	FString Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="URL"))
	FString Url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="API Token"))
	FString Token;
	
	bool IsValid() const
	{
		if (Token.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("MatchmakerConfig::No API Token Set"));
			return false;
		}
		if (Url.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("MatchmakerConfig::No URL Set"));
			return false;
		}
		return true;
	}
};