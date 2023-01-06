#include "CurrentTurnWidget.h"

#include "TicTacToeBoard.h"
#include "TicTacToePlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UCurrentTurnWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (_restartButton != nullptr)
	{
		_restartButton->OnClicked.AddDynamic(this, &UCurrentTurnWidget::Restart);
	}
}

void UCurrentTurnWidget::SetCurrentTurn(ECellState currentTurn) const
{
	if (_currentTurnText == nullptr)
	{
		return;
	}
	
	const FString turnString = FString::Printf(TEXT("Current Turn: %hs"), currentTurn == ECellState::X ? "X" : "O");
	_currentTurnText->SetText(FText::FromString(turnString));
}

void UCurrentTurnWidget::SetEnableRestartButton(bool value) const
{
	if (_restartButton == nullptr)
	{
		return;
	}

	_restartButton->SetIsEnabled(value);
}

void UCurrentTurnWidget::Restart()
{
	if (const ATicTacToePlayerController* playerController = Cast<ATicTacToePlayerController>(GetOwningPlayer()))
	{
		playerController->Server_RequestRestart();
	}
}
