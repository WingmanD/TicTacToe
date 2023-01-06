#include "TicTacToeHUD.h"

#include "CurrentTurnWidget.h"
#include "DeclareWinnerWidget.h"
#include "PauseMenuWidget.h"
#include "TicTacToePlayerController.h"
#include "WaitingForPlayersWidget.h"
#include "GameFramework/GameState.h"

ATicTacToeHUD::ATicTacToeHUD() {}

void ATicTacToeHUD::BeginPlay()
{
	Super::BeginPlay();

	if (_waitingForPlayersWidgetClass != nullptr)
	{
		_waitingForPlayersWidget = CreateWidget<UWaitingForPlayersWidget>(
			GetOwningPlayerController(), _waitingForPlayersWidgetClass);
		_waitingForPlayersWidget->AddToPlayerScreen();
	}

	if (_currentTurnWidgetClass != nullptr)
	{
		_currentTurnWidget = CreateWidget<UCurrentTurnWidget>(GetOwningPlayerController(), _currentTurnWidgetClass);
		_currentTurnWidget->SetEnableRestartButton(false);
		_currentTurnWidget->AddToPlayerScreen();
	}

	if (_pauseMenuWidgetClass != nullptr)
	{
		_pauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetOwningPlayerController(), _pauseMenuWidgetClass);
		_pauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		_pauseMenuWidget->AddToPlayerScreen();
	}

	if (const AGameState* gameState = GetWorld()->GetGameState<AGameState>())
	{
		const FName matchState = gameState->GetMatchState();
		if (matchState == "InProgress")
		{
			if (_waitingForPlayersWidget != nullptr)
			{
				if (const ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(
					GetOwningPlayerController()))
				{
					_waitingForPlayersWidget->ShowStartMessageAndHideAfterDelay(playerController->GetTicTacToeRole());
				}
			}
		}
	}
}

void ATicTacToeHUD::OnMatchStarted() const
{
	if (_waitingForPlayersWidget != nullptr)
	{
		if (const ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(
			GetOwningPlayerController()))
		{
			_waitingForPlayersWidget->ShowStartMessageAndHideAfterDelay(playerController->GetTicTacToeRole());
		}
	}

	if (_currentTurnWidget != nullptr)
	{
		_currentTurnWidget->SetEnableRestartButton(false);
	}
}

void ATicTacToeHUD::OnMatchFinished(ECellState winner) const
{
	DeclareWinner(winner);

	if (_currentTurnWidget != nullptr)
	{
		_currentTurnWidget->SetEnableRestartButton(true);
	}
}

void ATicTacToeHUD::OpenPauseMenu() const
{
	if (_pauseMenuWidget != nullptr)
	{
		_pauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATicTacToeHUD::SetCurrentTurn(ECellState currentTurn) const
{
	if (_currentTurnWidget != nullptr)
	{
		_currentTurnWidget->SetCurrentTurn(currentTurn);
	}
}

void ATicTacToeHUD::DeclareWinner(ECellState winner) const
{
	if (_declareWinnerWidgetClass != nullptr)
	{
		UDeclareWinnerWidget* declareWinnerWidget = CreateWidget<UDeclareWinnerWidget>(
			GetOwningPlayerController(), _declareWinnerWidgetClass);
		declareWinnerWidget->ShowWinnerMessageAndHideAfterDelay(winner);
		declareWinnerWidget->AddToPlayerScreen();
	}
}
