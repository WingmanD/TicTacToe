#include "DeclareWinnerWidget.h"

#include "TicTacToeBoard.h"
#include "Components/TextBlock.h"

void UDeclareWinnerWidget::ShowWinnerMessageAndHideAfterDelay(ECellState winner)
{
	SetVisibility(ESlateVisibility::Visible);

	if (_textBlock != nullptr)
	{
		const FString startMessage = FString::Printf(TEXT("%hs won!"), winner == ECellState::X ? "X" : "O");
		_textBlock->SetText(FText::FromString(startMessage));
	}

	TWeakObjectPtr<UDeclareWinnerWidget> weakThis = this;
	const FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([weakThis]()
	{
		if (weakThis != nullptr)
		{
			weakThis->RemoveFromParent();
		}
	});

	GetWorld()->GetTimerManager().SetTimer(_timerHandle, timerDelegate, _timeToHideMessage, false);
}
