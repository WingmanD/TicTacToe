#pragma once

#include "Blueprint/UserWidget.h"
#include "WaitingForPlayersWidget.generated.h"

enum class ECellState;
class UTextBlock;

UCLASS()
class TICTACTOE_API UWaitingForPlayersWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowStartMessageAndHideAfterDelay(ECellState currentTurn);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textBlock;

	UPROPERTY(EditDefaultsOnly, Category = "TicTacToe")
	float _timeToHideMessage = 3.0f;

	FTimerHandle _timerHandle;
};	
