#pragma once
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

namespace FHttpUtils
{
	inline FString GetErrorString(const FHttpRequestPtr& RequestPtr, const FHttpResponsePtr& ResponsePtr)
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
}
