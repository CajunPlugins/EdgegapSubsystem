#pragma once

#include "Utils/json.h"

#include "MatchmakingStructures.generated.h"

USTRUCT(BlueprintType)
struct EDGEGAPONLINESUBSYSTEM_API FTicketAssignment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Host;

	bool IsValid() const { return !Host.IsEmpty() && Host.Contains(":"); }
	
	FTicketAssignment(){}
	
	explicit FTicketAssignment(const TSharedPtr<FJsonObject>& JsonObject)
	{
		if (JsonObject->HasTypedField<EJson::String>("server_host"))
		{
			Host = JsonObject->GetStringField("server_host");
		}
	}
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
	
	FTicketData(){}
	
	explicit FTicketData(const TSharedPtr<FJsonObject>& JsonObject)
	{
		if (JsonObject->HasTypedField<EJson::String>("ticket_id"))
		{
			Id = JsonObject->GetStringField("ticket_id");
		}
		if (JsonObject->HasTypedField<EJson::Object>("assignment"))
		{
			Assignment = FTicketAssignment(JsonObject->GetObjectField("assignment"));
		}
	}
};

struct EDGEGAPONLINESUBSYSTEM_API FEdgegapResponse
{
	FString RequestId;
	TSharedPtr<FJsonObject> Data;

	template<typename TData>
	TData GetData() const { return TData(Data); }
	
	explicit FEdgegapResponse(const TSharedPtr<FJsonObject>& JsonObject)
	{
		RequestId = JsonObject->GetStringField("request_id");
		Data = JsonObject->GetObjectField("data");
	}

	explicit FEdgegapResponse(const FString& JsonString)
	{
		const TSharedPtr<FJsonObject>& JsonObject = FJsonUtils::StringToJsonObject(JsonString);
		RequestId = JsonObject->GetStringField("request_id");
		Data = JsonObject->GetObjectField("data");
	}

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

	TSharedPtr<FJsonObject> ToJson() const
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField("edgegap_profile_id", *ProfileId);
		TSharedPtr<FJsonObject> MatchmakingJsonObject = MakeShareable(new FJsonObject);
		MatchmakingJsonObject->SetObjectField("selector_data", FJsonUtils::MapToJsonObject(Selectors));
		MatchmakingJsonObject->SetObjectField("filter_data", FJsonUtils::MapToJsonObject(Filters));
		JsonObject->SetObjectField("matchmaking_data", MatchmakingJsonObject);
		return JsonObject;
	}

	FString ToJsonString() const { return FJsonUtils::JsonObjectToString(ToJson()); }
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