#pragma once

#include "GameFramework/HUD.h"
#include "TicTacToeHUD.generated.h"

class UDeclareWinnerWidget;
enum class ECellState;
class UWaitingForPlayersWidget;
class UCurrentTurnWidget;
class UPauseMenuWidget;

UCLASS()
class TICTACTOE_API ATicTacToeHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATicTacToeHUD();
	
	virtual void BeginPlay() override;

	void OnMatchStarted() const;
	void OnMatchFinished(ECellState winner) const;

	void OpenPauseMenu() const;

	void SetCurrentTurn(ECellState currentTurn) const;

	void DeclareWinner(ECellState winner) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "TicTacToe")
	TSubclassOf<UWaitingForPlayersWidget> _waitingForPlayersWidgetClass;

	UPROPERTY()
	UWaitingForPlayersWidget* _waitingForPlayersWidget;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "TicTacToe")
	TSubclassOf<UCurrentTurnWidget> _currentTurnWidgetClass;

	UPROPERTY()
	UCurrentTurnWidget* _currentTurnWidget;
	

	UPROPERTY(EditDefaultsOnly, Category = "TicTacToe")
	TSubclassOf<UPauseMenuWidget> _pauseMenuWidgetClass;

	UPROPERTY()
	UPauseMenuWidget* _pauseMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "TicTacToe")
	TSubclassOf<UDeclareWinnerWidget> _declareWinnerWidgetClass;
};
