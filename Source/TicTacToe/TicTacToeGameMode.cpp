#include "TicTacToeGameMode.h"

#include "TicTacToePlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

bool ATicTacToeGameMode::ReadyToStartMatch_Implementation()
{
	if (const AGameStateBase* gameState = GetGameState<AGameStateBase>())
	{
		return gameState->PlayerArray.Num() == _requiredPlayers;
	}

	return false;
}

void ATicTacToeGameMode::StartMatch()
{
	if (AGameStateBase* gameState = GetGameState<AGameStateBase>())
	{
		if (gameState->PlayerArray.Num() != _requiredPlayers)
		{
			UE_LOG(LogTemp, Error, TEXT("TicTacToeGameModeBase::StartPlay: Invalid number of players"));
			return;
		}

		const bool isFirstPlayerX = FMath::RandBool();

		if (isFirstPlayerX)
		{
			_currentRole = ECellState::X;
		}
		else
		{
			_currentRole = ECellState::O;
		}

		if (const APlayerState* playerState = gameState->PlayerArray[0])
		{
			if (ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(
				playerState->GetOwner()))
			{
				playerController->SetIsPlayerTurn(isFirstPlayerX);
				playerController->SetTicTacToeRole(isFirstPlayerX ? ECellState::X : ECellState::O);
			}
		}

		if (const APlayerState* playerState = gameState->PlayerArray[1])
		{
			if (ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(
				playerState->GetOwner()))
			{
				playerController->SetIsPlayerTurn(!isFirstPlayerX);
				playerController->SetTicTacToeRole(isFirstPlayerX ? ECellState::O : ECellState::X);
			}
		}

		NotifyMatchStarted();
	}

	Super::StartMatch();
}

void ATicTacToeGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	RestartGame();
}

void ATicTacToeGameMode::PlayerFinishedTurn()
{
	AGameStateBase* gameState = GetGameState<AGameStateBase>();
	if (gameState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TicTacToeGameModeBase::PlayerFinishedTurn: Invalid game state"));
		return;
	}

	if (gameState->PlayerArray.Num() != _requiredPlayers)
	{
		UE_LOG(LogTemp, Error, TEXT("TicTacToeGameModeBase::PlayerFinishedTurn: Invalid number of players"));
		return;
	}

	if (const APlayerState* playerState = gameState->PlayerArray[0])
	{
		if (ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(
			playerState->GetOwner()))
		{
			playerController->SetIsPlayerTurn(!playerController->GetIsPlayerTurn());
		}
	}

	if (const APlayerState* playerState = gameState->PlayerArray[1])
	{
		if (ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(
			playerState->GetOwner()))
		{
			playerController->SetIsPlayerTurn(!playerController->GetIsPlayerTurn());
		}
	}

	if (_currentRole == ECellState::X)
	{
		_currentRole = ECellState::O;
	}
	else if (_currentRole == ECellState::O)
	{
		_currentRole = ECellState::X;
	}
}

void ATicTacToeGameMode::FinishedGame()
{
	NotifyMatchFinished();
}

void ATicTacToeGameMode::Restart()
{
	TArray<AActor*> boards;
	UGameplayStatics::GetAllActorsOfClass(this, ATicTacToeBoard::StaticClass(), boards);

	for (AActor* board : boards)
	{
		if (ATicTacToeBoard* ticTacToeBoard = Cast<ATicTacToeBoard>(board))
		{
			ticTacToeBoard->NetMulticast_ClearBoard();
		}
	}

	//PlayerFinishedTurn();
	NotifyMatchStarted();
}

void ATicTacToeGameMode::NotifyMatchStarted() const
{
	AGameStateBase* gameState = GetGameState<AGameStateBase>();
	if (gameState == nullptr)
	{
		return;
	}

	for (const APlayerState* playerState : gameState->PlayerArray)
	{
		if (const ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(
			playerState->GetPlayerController()))
		{
			playerController->Client_OnMatchStarted();
		}
	}
}

void ATicTacToeGameMode::NotifyMatchFinished() const
{
	AGameStateBase* gameState = GetGameState<AGameStateBase>();
	if (gameState == nullptr)
	{
		return;
	}

	for (const APlayerState* playerState : gameState->PlayerArray)
	{
		if (const ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(
			playerState->GetPlayerController()))
		{
			playerController->Client_OnMatchFinished(_currentRole);
		}
	}
}
