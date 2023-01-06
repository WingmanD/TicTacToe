#include "WaitingForPlayersWidget.h"

#include "TicTacToeBoard.h"
#include "Components/TextBlock.h"

void UWaitingForPlayersWidget::ShowStartMessageAndHideAfterDelay(ECellState currentTurn)
{
	SetVisibility(ESlateVisibility::Visible);

	if (_textBlock != nullptr)
	{
		const FString startMessage = FString::Printf(TEXT("You will play as %hs"), currentTurn == ECellState::X ? "X" : "O");
		_textBlock->SetText(FText::FromString(startMessage));
	}

	TWeakObjectPtr<UWaitingForPlayersWidget> weakThis = this;
	const FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([weakThis]()
	{
		if (weakThis != nullptr)
		{
			weakThis->SetVisibility(ESlateVisibility::Collapsed);
		}
	});

	GetWorld()->GetTimerManager().SetTimer(_timerHandle, timerDelegate, _timeToHideMessage, false);
}
