#pragma once

#include "GameFramework/GameMode.h"
#include "TicTacToeGameMode.generated.h"

class ATicTacToeBoard;
enum class ECellState;

UCLASS()
class TICTACTOE_API ATicTacToeGameMode : public AGameMode
{
	GENERATED_BODY()

	// AGameMode
public:
	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void StartMatch() override;

	virtual void Logout(AController* Exiting) override;

public:
	[[nodiscard]] ECellState GetCurrentRole() const
	{
		return _currentRole;
	}

	void PlayerFinishedTurn();

	void FinishedGame();

	void Restart();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 _requiredPlayers = 2;

	ECellState _currentRole;

	void NotifyMatchStarted() const;
	void NotifyMatchFinished() const;
};
