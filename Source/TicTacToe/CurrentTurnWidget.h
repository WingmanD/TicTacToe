#pragma once

#include "Blueprint/UserWidget.h"
#include "CurrentTurnWidget.generated.h"

class UButton;
enum class ECellState;
class UTextBlock;

UCLASS()
class TICTACTOE_API UCurrentTurnWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetCurrentTurn(ECellState currentTurn) const;

	void SetEnableRestartButton(bool value) const;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _currentTurnText;

	UPROPERTY(meta = (BindWidget))
	UButton* _restartButton;

private:
	UFUNCTION()
	void Restart();
};
