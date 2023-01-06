#pragma once

#include "InputAction.h"
#include "TicTacToeBoard.h"
#include "GameFramework/PlayerController.h"
#include "TicTacToePlayerController.generated.h"

struct FInputActionValue;

UCLASS()
class TICTACTOE_API ATicTacToePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATicTacToePlayerController();

	UFUNCTION(Client, Reliable)
	void Client_OnMatchStarted() const;

	UFUNCTION(Client, Reliable)
	void Client_OnMatchFinished(ECellState winner) const;

	UFUNCTION(Server, Reliable)
	void Server_RequestRestart() const;

	[[nodiscard]] ECellState GetTicTacToeRole() const
	{
		return _ticTacToeRole;
	}

	void SetTicTacToeRole(ECellState value)
	{
		if (_ticTacToeRole == value)
		{
			return;
		}

		_ticTacToeRole = value;
	}

	void SetIsPlayerTurn(bool value);

	bool GetIsPlayerTurn() const;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* _inputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* _clickAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* _pauseMenuAction;

	UPROPERTY(Replicated)
	ECellState _ticTacToeRole = ECellState::X;

	UPROPERTY(ReplicatedUsing = OnRep_IsPlayerTurn)
	bool _isPlayerTurn;

private:
	void Click(const FInputActionValue& value);
	void OpenPauseMenu(const FInputActionValue& value);

	UFUNCTION(Server, Reliable)
	void Server_Click(const FHitResult& hitResult);

	void OnPlayerTurnChanged() const;

	UFUNCTION()
	void OnRep_IsPlayerTurn() const;
};
