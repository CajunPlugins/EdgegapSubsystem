// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#include "EdgegapSubsystem.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "Utils/HTTPUtils.h"
#include "Utils/JSONUtils.h"

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
		EGSS_LOG(Error, TEXT("CreateTicket::Invalid Config"));
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
			EGSS_LOG(Error, TEXT("GetTicket::Invalid Config"));
			OnError.Broadcast("InvalidConfig");;
		}
	} else
	{
		EGSS_LOG(Error, TEXT("GetTicket::Empty TicketId"));
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
			EGSS_LOG(Error, TEXT("DeleteTicket::Invalid Config"));
			OnError.Broadcast("InvalidConfig");;
		}
	} else
	{
		EGSS_LOG(Error, TEXT("DeleteTicket::Empty TicketId"));
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
void UEdgegapSubsystem::HandleCreateTicket(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		EGSS_LOG(Log, TEXT("HandleCreateTicketResponse::%s"), *ResponsePtr->GetContentAsString());
		OnTicketCreated.Broadcast( FEdgegapResponse::FromJson(ResponsePtr->GetContentAsString()).GetData<FTicketData>());
	} else
	{
		EGSS_LOG(Error, TEXT("HandleCreateTicketResponse::%s"), *FHttpUtils::GetErrorString(RequestPtr, ResponsePtr));
		OnError.Broadcast(*FHttpUtils::GetErrorString(RequestPtr, ResponsePtr));
	}
}

// ReSharper disable once CppPassValueParameterByConstReference
void UEdgegapSubsystem::HandleGetTicket(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		EGSS_LOG(Log, TEXT("HandleGetTicketResponse::%s"), *ResponsePtr->GetContentAsString());
		OnTicketRetrieved.Broadcast(FEdgegapResponse::FromJson(ResponsePtr->GetContentAsString()).GetData<FTicketData>());
	} else
	{
		EGSS_LOG(Error, TEXT("HandleGetTicketResponse::%s"), *FHttpUtils::GetErrorString(RequestPtr, ResponsePtr));
		OnError.Broadcast(*FHttpUtils::GetErrorString(RequestPtr, ResponsePtr));
	}
}


// ReSharper disable once CppPassValueParameterByConstReference
void UEdgegapSubsystem::HandleDeleteTicket(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		EGSS_LOG(Log, TEXT("HandleDeleteTicket::%s"), *ResponsePtr->GetContentAsString());
		OnTicketDeleted.Broadcast(FEdgegapResponse::FromJson(ResponsePtr->GetContentAsString()).GetData<FTicketData>());
	} else
	{
		EGSS_LOG(Error, TEXT("HandleDeleteTicket::%s"), *FHttpUtils::GetErrorString(RequestPtr, ResponsePtr));
		OnError.Broadcast(*FHttpUtils::GetErrorString(RequestPtr, ResponsePtr));
	}
}

TSharedRef<IHttpRequest> UEdgegapSubsystem::CreateRequest(const FMatchmakerConfig& Config, const FString& Endpoint, const FString& Method)
{
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
	Request->SetVerb(Method);
	Request->SetHeader("Authorization", *Config.Token);
	Request->SetHeader("Content-Type", "application/json");
	Request->SetURL(FString::Printf(TEXT("%s/v1%s"), *Config.Url, *Endpoint));
	Request->SetTimeout(10);
	return Request;
}

UEdgegapSubsystem* UEdgegapSubsystem::Get(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			return World->GetGameInstance()->GetSubsystem<UEdgegapSubsystem>();
		}
	}
	return nullptr;
}