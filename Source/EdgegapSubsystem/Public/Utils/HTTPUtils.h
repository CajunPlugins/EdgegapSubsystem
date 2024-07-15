// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#pragma once

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

struct EDGEGAPSUBSYSTEM_API FHttpUtils
{
	FHttpUtils() = delete;
	
	static FString GetErrorString(const FHttpRequestPtr& RequestPtr, const FHttpResponsePtr& ResponsePtr)
	{
		FString Error = "HTTP Request Error";
		if (ResponsePtr.IsValid())
		{
			Error = ResponsePtr->GetContentAsString();
		} else if (RequestPtr.IsValid())
		{
			Error =  EHttpRequestStatus::ToString(RequestPtr->GetStatus());
		}
		return Error;
	}
};
