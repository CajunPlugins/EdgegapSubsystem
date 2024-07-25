// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#pragma once

namespace Errors
{
	const FString TicketNotFound = TEXT("TICKET_NOT_FOUND");
	const FString InvalidRequest = TEXT("INVALID_REQUEST");
	const FString CreateTicketFailure = TEXT("CREATE_TICKET_FAILURE");

	static bool IsEdgegapError(const FString& Error) { return Error == TicketNotFound || Error == InvalidRequest || Error == CreateTicketFailure; }
}