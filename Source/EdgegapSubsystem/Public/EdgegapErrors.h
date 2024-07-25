// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#pragma once

namespace Errors
{
	constexpr const wchar_t* TicketNotFound = TEXT("TICKET_NOT_FOUND");
	constexpr const wchar_t* InvalidRequest = TEXT("INVALID_REQUEST");
	constexpr const wchar_t* CreateTicketFailure = TEXT("CREATE_TICKET_FAILURE");

	static bool IsEdgegapError(const FString& Error) { return Error == TicketNotFound || Error == InvalidRequest || Error == CreateTicketFailure; }
}