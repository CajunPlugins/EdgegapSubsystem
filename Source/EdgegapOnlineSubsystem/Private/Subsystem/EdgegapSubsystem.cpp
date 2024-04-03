// Copyright 2024 Rise Of A Legend.


#include "Subsystem/EdgegapSubsystem.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "Utils/json.h"

DEFINE_LOG_CATEGORY(LogEdgegapSubsystem);

void UEdgegapSubsystem::CreateTicket(const FMatchmakerConfig& Config, const FMatchmakerSettings& Settings)
{
	if (Config.IsValid())
	{
		const TSharedRef<IHttpRequest> Request = CreateRequest(Config, "/tickets", "POST");
		Request->SetContentAsString(Settings.ToJsonString());
		Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleCreateTicket);
		Request->ProcessRequest();
	} else
	{
		EG_LOG(Error, TEXT("CreateTicket::Invalid Config"));
		OnError.Broadcast("InvalidConfig");
	}
}

void UEdgegapSubsystem::GetTicket(const FMatchmakerConfig& Config, const FString& TicketId)
{
	if (!TicketId.IsEmpty())
	{
		if (Config.IsValid())
		{
			const TSharedRef<IHttpRequest> Request = CreateRequest(Config, FString::Printf(TEXT("/tickets/%s"), *TicketId));
			Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleGetTicket);
			Request->ProcessRequest();
		} else
		{
			EG_LOG(Error, TEXT("GetTicket::Invalid Config"));
			OnError.Broadcast("InvalidConfig");;
		}
	} else
	{
		EG_LOG(Error, TEXT("GetTicket::Empty TicketId"));
		OnError.Broadcast("EmptyTicketId");
	}
	
}

void UEdgegapSubsystem::DeleteTicket(const FMatchmakerConfig& Config, const FString& TicketId)
{
	if (!TicketId.IsEmpty())
	{
		if (Config.IsValid())
		{
			const TSharedRef<IHttpRequest> Request = CreateRequest(Config, FString::Printf(TEXT("/tickets/%s"), *TicketId), "DELETE");
			Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleDeleteTicket);
			Request->ProcessRequest();
		} else
		{
			EG_LOG(Error, TEXT("DeleteTicket::Invalid Config"));
			OnError.Broadcast("InvalidConfig");;
		}
	} else
	{
		EG_LOG(Error, TEXT("DeleteTicket::Empty TicketId"));
		OnError.Broadcast("EmptyTicketId");
	}
}

AMatchmaker* UEdgegapSubsystem::GetMatchmaker(const FString& Id)
{
	return RegisteredMatchmakers.Contains(Id) ? RegisteredMatchmakers[Id] : nullptr;
}

void UEdgegapSubsystem::RegisterMatchmaker(const FString& Id, AMatchmaker* Matchmaker)
{
	RegisteredMatchmakers.Emplace(Id, Matchmaker);
}

void UEdgegapSubsystem::UnregisterMatchmaker(const FString& Id)
{
	RegisteredMatchmakers.Remove(Id);
}

// ReSharper disable once CppPassValueParameterByConstReference
void UEdgegapSubsystem::HandleCreateTicket(FHttpRequestPtr, FHttpResponsePtr Response, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		const FString StringResponse = Response->GetContentAsString();
		EG_LOG(Log, TEXT("HandleCreateTicketResponse::%s"), *StringResponse);
		const FEdgegapResponse EdgegapResponse = FEdgegapResponse(StringResponse);
		const FTicketData TicketData = EdgegapResponse.GetData<FTicketData>();
		OnTicketCreated.Broadcast(TicketData);
	} else
	{
		EG_LOG(Error, TEXT("HandleCreateTicketResponse::%s"), *Response->GetContentAsString());
		OnError.Broadcast(*Response->GetContentAsString());
	}
}

// ReSharper disable once CppPassValueParameterByConstReference
void UEdgegapSubsystem::HandleGetTicket(FHttpRequestPtr, FHttpResponsePtr Response, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		const FString StringResponse = Response->GetContentAsString();
		EG_LOG(Log, TEXT("HandleGetTicketResponse::%s"), *StringResponse);
		const FEdgegapResponse EdgegapResponse = FEdgegapResponse(StringResponse);
		const FTicketData TicketData = EdgegapResponse.GetData<FTicketData>();
		OnTicketRetrieved.Broadcast(TicketData);
	} else
	{
		EG_LOG(Error, TEXT("HandleGetTicketResponse::%s"), *Response->GetContentAsString());
		OnError.Broadcast(*Response->GetContentAsString());
	}
}


// ReSharper disable once CppPassValueParameterByConstReference
void UEdgegapSubsystem::HandleDeleteTicket(FHttpRequestPtr, FHttpResponsePtr Response, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		const FString StringResponse = Response->GetContentAsString();
		EG_LOG(Log, TEXT("HandleDeleteTicket::%s"), *StringResponse);
		const FEdgegapResponse EdgegapResponse = FEdgegapResponse(StringResponse);
		const FTicketData TicketData = EdgegapResponse.GetData<FTicketData>();
		OnTicketDeleted.Broadcast(TicketData);
	} else
	{
		EG_LOG(Error, TEXT("HandleDeleteTicket::%s"), *Response->GetContentAsString());
		OnError.Broadcast(*Response->GetContentAsString());
	}
}

TSharedRef<IHttpRequest> UEdgegapSubsystem::CreateRequest(const FMatchmakerConfig& Config, const FString& Endpoint, const FString& Method)
{
	FHttpModule& HttpModule = FHttpModule::Get();
	const TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
	Request->SetVerb(Method);
	Request->SetHeader("Authorization", *Config.Token);
	Request->SetHeader("Content-Type", "application/json");
	Request->SetURL(FString::Printf(TEXT("%s/v1%s"), *Config.Url, *Endpoint));
	return Request;
}
