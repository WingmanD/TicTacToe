#pragma once

#include "GameFramework/Actor.h"
#include "TicTacToeBoard.generated.h"

UENUM()
enum class ECellState { Empty = 0, X, O };

USTRUCT()
struct FBoardRow
{
	GENERATED_BODY()

	TArray<ECellState> Cells;

	ECellState& operator[](int32 Index)
	{
		return Cells[Index];
	}

	const ECellState& operator[](int32 Index) const
	{
		return Cells[Index];
	}
};

USTRUCT()
struct FTicTacToeField
{
	GENERATED_BODY()

	TArray<FBoardRow> Rows;

	void Init(const FIntVector2& size)
	{
		Rows.SetNum(size.Y);
		for (int32 i = 0; i < size.Y; ++i)
		{
			Rows[i].Cells.SetNum(size.X);
			for (int32 j = 0; j < size.X; ++j)
			{
				Rows[i][j] = ECellState::Empty;
			}
		}
	}

	void Clear()
	{
		const FIntVector2 size = FIntVector2(Rows[0].Cells.Num(), Rows.Num());
		Rows.SetNum(size.Y);
		for (int32 i = 0; i < size.Y; ++i)
		{
			Rows[i].Cells.SetNum(size.X);
			for (int32 j = 0; j < size.X; ++j)
			{
				Rows[i][j] = ECellState::Empty;
			}
		}
	}

	bool CheckColumnFinished(int32 rowIndex) const
	{
		const FBoardRow& row = Rows[rowIndex];
		const ECellState firstCell = row[0];

		if (firstCell == ECellState::Empty)
		{
			return false;
		}

		for (int32 i = 1; i < row.Cells.Num(); ++i)
		{
			if (row[i] != firstCell)
			{
				return false;
			}
		}

		return true;
	}

	bool CheckRowFinished(int32 columnIndex) const
	{
		const ECellState firstCell = Rows[0][columnIndex];

		if (firstCell == ECellState::Empty)
		{
			return false;
		}

		for (int32 i = 1; i < Rows.Num(); ++i)
		{
			if (Rows[i][columnIndex] != firstCell)
			{
				return false;
			}
		}

		return true;
	}

	bool CheckLeftDiagonalFinished() const
	{
		const ECellState firstCell = Rows[0][0];

		if (firstCell == ECellState::Empty)
		{
			return false;
		}

		for (int32 i = 1; i < Rows.Num(); ++i)
		{
			if (Rows[i][i] != firstCell)
			{
				return false;
			}
		}

		return true;
	}

	bool CheckRightDiagonalFinished() const
	{
		const ECellState firstCell = Rows[0][Rows.Num()- 1];

		if (firstCell == ECellState::Empty)
		{
			return false;
		}

		for (int32 i = 1; i < Rows.Num(); ++i)
		{
			if (Rows[i][Rows.Num() - i - 1] != firstCell)
			{
				return false;
			}
		}

		return true;
	}


	FBoardRow& operator[](int32 Index)
	{
		return Rows[Index];
	}

	const FBoardRow& operator[](int32 Index) const
	{
		return Rows[Index];
	}
};

UCLASS()
class TICTACTOE_API ATicTacToeBoard : public AActor
{
	GENERATED_BODY()

public:
	ATicTacToeBoard();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetTileState(const FVector& location, ECellState state);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ClearBoard();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* _boardMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USceneComponent* _boardRoot;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UInstancedStaticMeshComponent* _xMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UInstancedStaticMeshComponent* _oMesh;

	UPROPERTY(Replicated)
	FTicTacToeField _board;

	UPROPERTY(EditDefaultsOnly, Category = "TicTacToe|Board")
	float _tileSize = 33.0f;

	bool _isFinished = false;

private:
	void SetTileState(const FIntVector2& index, ECellState state);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_UpdateMeshes(const FIntVector2& index, ECellState state);

	FIntVector2 GetTileIndex(const FVector& worldLocation) const;

	FVector GetTileLocation(const FIntVector2& index) const;

	bool CheckIsFinished(const FIntVector2& lastChangedIndex);
};
