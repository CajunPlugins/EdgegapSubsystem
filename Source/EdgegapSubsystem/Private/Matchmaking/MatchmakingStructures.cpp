// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#include "Matchmaking/MatchmakingStructures.h"
#include "Dom/JsonObject.h"
#include "Matchmaking/EdgegapSubsystemMatchmakingMacros.h"
#include "Utils/JSONUtils.h"

FTicketAssignment FTicketAssignment::FromJson(const TSharedPtr<FJsonObject>& JsonObject)

{
	FTicketAssignment TicketAssignment;
	if (JsonObject->HasTypedField<EJson::String>(TEXT("server_host")))
	{
		TicketAssignment.Host = JsonObject->GetStringField(TEXT("server_host"));
	}
	return TicketAssignment;
}

FTicketData FTicketData::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FTicketData TicketData;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("ticket_id")))
		{
			TicketData.Id = JsonObject->GetStringField(TEXT("ticket_id"));
		}
		else if (JsonObject->HasTypedField<EJson::String>(TEXT("id")))
		{
			TicketData.Id = JsonObject->GetStringField(TEXT("id"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("assignment")))
		{
			TicketData.Assignment = FTicketAssignment::FromJson(JsonObject->GetObjectField(TEXT("assignment")));
		}
	}
	return TicketData;
}

FEdgegapResponse FEdgegapResponse::FromJson(const TSharedPtr<FJsonObject>& JsonObject) 	{
	FEdgegapResponse EdgegapResponse;
	if (JsonObject->HasTypedField<EJson::String>(TEXT("request_id")))
	{
		EdgegapResponse.RequestId = JsonObject->GetStringField(TEXT("request_id"));
	}
	if (JsonObject->HasTypedField<EJson::Object>(TEXT("data")))
	{
		EdgegapResponse.Data = JsonObject->GetObjectField(TEXT("data"));
	}
	if (JsonObject->HasTypedField<EJson::String>(TEXT("error")))
	{
		EdgegapResponse.Error = JsonObject->GetStringField(TEXT("error"));
	}
	return EdgegapResponse;
}

FEdgegapResponse FEdgegapResponse::FromJson(const FString& JsonString) { return FromJson(FJsonUtils::StringToJsonObject(JsonString)); }

TSharedPtr<FJsonObject> FMatchmakerSettings::ToJson() const {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("edgegap_profile_id"), *ProfileId);
	JsonObject->SetStringField(TEXT("player_id"), *PlayerId);
	TSharedPtr<FJsonObject> MatchmakingJsonObject = MakeShareable(new FJsonObject);
	MatchmakingJsonObject->SetArrayField(TEXT("selector_data"), FJsonUtils::ArrayToJsonValues(Selectors));
	MatchmakingJsonObject->SetObjectField(TEXT("filter_data"), FJsonUtils::MapToJsonObject(Filters));
	JsonObject->SetObjectField(TEXT("matchmaking_data"), MatchmakingJsonObject);
	return JsonObject;
}

FString FMatchmakerSettings::ToJsonString() const { return FJsonUtils::JsonObjectToString(ToJson()); }

bool FMatchmakerConfig::IsValid() const
{
	if (Token.IsEmpty())
	{
		EGMM_LOG(Error, TEXT("MatchmakerConfig::No API Token Set"));
		return false;
	}
	if (Url.IsEmpty())
	{
		EGMM_LOG(Error, TEXT("MatchmakerConfig::No URL Set"));
		return false;
	}
	return true;
}