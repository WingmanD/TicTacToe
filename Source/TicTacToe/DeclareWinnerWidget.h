#pragma once

#include "Blueprint/UserWidget.h"
#include "DeclareWinnerWidget.generated.h"

enum class ECellState;
class UTextBlock;

UCLASS()
class TICTACTOE_API UDeclareWinnerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowWinnerMessageAndHideAfterDelay(ECellState winner);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textBlock;

	UPROPERTY(EditDefaultsOnly, Category = "TicTacToe")
	float _timeToHideMessage = 3.0f;

	FTimerHandle _timerHandle;
};	
