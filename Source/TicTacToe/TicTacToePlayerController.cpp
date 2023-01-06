#include "TicTacToePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TicTacToeBoard.h"
#include "TicTacToeGameMode.h"
#include "TicTacToeHUD.h"
#include "Net/UnrealNetwork.h"

ATicTacToePlayerController::ATicTacToePlayerController()
{
	bShowMouseCursor = true;
	AController::SetIgnoreMoveInput(true);
	AController::SetIgnoreLookInput(true);
}

void ATicTacToePlayerController::Server_RequestRestart_Implementation() const
{
	if (ATicTacToeGameMode* gameMode = Cast<ATicTacToeGameMode>(GetWorld()->GetAuthGameMode()))
	{
		gameMode->Restart();
	}
}

void ATicTacToePlayerController::Client_OnMatchStarted_Implementation() const
{
	if (const ATicTacToeHUD* HUD = Cast<ATicTacToeHUD>(GetHUD()))
	{
		HUD->OnMatchStarted();
	}
}

void ATicTacToePlayerController::Client_OnMatchFinished_Implementation(ECellState winner) const
{
	if (const ATicTacToeHUD* HUD = Cast<ATicTacToeHUD>(GetHUD()))
	{
		HUD->OnMatchFinished(winner);
	}
}


void ATicTacToePlayerController::SetIsPlayerTurn(bool value)
{
	if (_isPlayerTurn == value)
	{
		return;
	}

	_isPlayerTurn = value;

	OnPlayerTurnChanged();
}

bool ATicTacToePlayerController::GetIsPlayerTurn() const
{
	return _isPlayerTurn;
}

void ATicTacToePlayerController::BeginPlay()
{
	Super::BeginPlay();


	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(_inputMappingContext, 0);
	}
}

void ATicTacToePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		enhancedInputComponent->BindAction(_clickAction, ETriggerEvent::Triggered, this,
		                                   &ATicTacToePlayerController::Click);
		enhancedInputComponent->BindAction(_pauseMenuAction, ETriggerEvent::Triggered, this,
										   &ATicTacToePlayerController::OpenPauseMenu);
	}
}

void ATicTacToePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATicTacToePlayerController, _ticTacToeRole);
	DOREPLIFETIME(ATicTacToePlayerController, _isPlayerTurn);
}

void ATicTacToePlayerController::Click(const FInputActionValue& value)
{
	FHitResult hitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, hitResult);

	Server_Click(hitResult);
}

void ATicTacToePlayerController::OpenPauseMenu(const FInputActionValue& value)
{
	if (const ATicTacToeHUD* HUD = Cast<ATicTacToeHUD>(GetHUD()))
	{
		HUD->OpenPauseMenu();
	}
}

void ATicTacToePlayerController::Server_Click_Implementation(const FHitResult& hitResult)
{
	if (hitResult.bBlockingHit)
	{
		if (ATicTacToeBoard* board = Cast<ATicTacToeBoard>(hitResult.GetActor()))
		{
			board->SetTileState(hitResult.Location, _ticTacToeRole);
		}
	}
}

void ATicTacToePlayerController::OnPlayerTurnChanged() const
{
	if (const ATicTacToeHUD* HUD = Cast<ATicTacToeHUD>(GetHUD()))
	{
		if (_isPlayerTurn)
		{
			HUD->SetCurrentTurn(GetTicTacToeRole());
		}
		else
		{
			HUD->SetCurrentTurn(GetTicTacToeRole() == ECellState::X ? ECellState::O : ECellState::X);
		}
	}
}

void ATicTacToePlayerController::OnRep_IsPlayerTurn() const
{
	OnPlayerTurnChanged();
}
