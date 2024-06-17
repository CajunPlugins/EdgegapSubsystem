#include "Matchmaking/Matchmaker.h"
#include "Subsystem/EdgegapSubsystem.h"

// Sets default values
AMatchmaker::AMatchmaker()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SetCanBeDamaged(false);
}

void AMatchmaker::StartMatchmaking()
{
	if (!bMatchmaking)
	{
		if (Config.IsValid())
		{
			bMatchmaking = true;
			UEdgegapSubsystem::Get(this)->CreateTicket(Config, Settings);
		} else
		{
			OnError("Invalid config");
		}
	} else
	{
		OnError("Already matchmaking");
	}
}

void AMatchmaker::CancelMatchmaking()
{
	if (bMatchmaking)
	{
		UEdgegapSubsystem::Get(this)->DeleteTicket(Config, CurrentTicketData.Id);
	} else
	{
		OnError("Not matchmaking");
	}
}

void AMatchmaker::SetSettings(const FMatchmakerSettings& NewSettings)
{
	if (!bMatchmaking)
	{
		Settings = NewSettings;
	} else
	{
		EG_LOG(Error, TEXT("SetSettings:Settings cannot be changed while matchmaking"));
	}
}

void AMatchmaker::SetProfileId(const FString& NewProfileId)
{
	if (!bMatchmaking)
	{
		Settings.ProfileId = NewProfileId;
	} else
	{
		EG_LOG(Error, TEXT("SetProfileId::ProfileId cannot be changed while matchmaking"))
	}
}

void AMatchmaker::SetFilters(const TMap<FString, FString>& NewFilters)
{
	if (!bMatchmaking)
	{
		Settings.Filters = NewFilters;
	} else
	{
		EG_LOG(Error, TEXT("SetFilters::Filters cannot be changed while matchmaking"))
	}
}

void AMatchmaker::SetSelectors(const TMap<FString, FString>& NewSelectors)
{
	if (!bMatchmaking)
	{
		Settings.Selectors = NewSelectors;
	} else
	{
		EG_LOG(Error, TEXT("SetSelectors::Selectors cannot be changed while matchmaking"))
	}
}

void AMatchmaker::ConnectToMatch(const FString& UrlOptions)
{

	GetWorld()->GetFirstPlayerController()->ClientTravel(FString::Printf(TEXT("%s%s"), *CurrentTicketData.Assignment.Host, *UrlOptions), TRAVEL_Absolute, false);
}

bool AMatchmaker::IsMatchmaking() const
{
	return bMatchmaking;
}

// Called when the game starts or when spawned
void AMatchmaker::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEdgegapSubsystem* Subsystem = UEdgegapSubsystem::Get(this))
	{
	    Subsystem->OnTicketCreated.AddDynamic(this, &ThisClass::HandleTicketCreated);
	    Subsystem->OnTicketRetrieved.AddDynamic(this, &ThisClass::HandleTicketRetrieved);
	    Subsystem->OnTicketDeleted.AddDynamic(this, &ThisClass::HandleTicketDeleted);
	    Subsystem->OnError.AddDynamic(this, &ThisClass::HandleError);
	    Subsystem->RegisterMatchmaker(Config.Id, this);
	}
	else
	{
		Destroy();
	}
}

void AMatchmaker::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UEdgegapSubsystem* Subsystem = UEdgegapSubsystem::Get(this))
	{
		Subsystem->OnTicketCreated.RemoveAll(this);
		Subsystem->OnTicketRetrieved.RemoveAll(this);
		Subsystem->OnTicketDeleted.RemoveAll(this);
		Subsystem->OnError.RemoveAll(this);
		Subsystem->UnregisterMatchmaker(Config.Id);
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AMatchmaker::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMatchmaker::HandleTicketCreated(const FTicketData TicketData)
{
	CurrentTicketData = TicketData;
	GetWorldTimerManager().SetTimer(MatchStatusTimerHandle, this, &ThisClass::PollCurrentProgress, 10.0f, true, 10.0f);
}

void AMatchmaker::HandleTicketRetrieved(const FTicketData TicketData)
{
	CurrentTicketData = TicketData;
	if (CurrentTicketData.IsAssigned())
	{
		HandleMatchFound();
	} else
	{
		OnProgress();
	}
}

void AMatchmaker::HandleTicketDeleted(FTicketData TicketData)
{
	GetWorldTimerManager().ClearTimer(MatchStatusTimerHandle);
	bMatchmaking = false;
	CurrentTicketData = FTicketData();
	OnCancelled();
}

void AMatchmaker::HandleMatchFound()
{
	GetWorldTimerManager().ClearTimer(MatchStatusTimerHandle);
	bMatchmaking = false;
	OnMatchFound(CurrentTicketData.Assignment.Host, CurrentTicketData.Id);
}

void AMatchmaker::HandleError(const FString Reason)
{
	OnError(Reason);
}

void AMatchmaker::PollCurrentProgress()
{
	UEdgegapSubsystem::Get(this)->GetTicket(Config, CurrentTicketData.Id);
}

