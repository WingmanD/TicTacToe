#include "TicTacToeBoard.h"

#include "TicTacToeGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATicTacToeBoard::ATicTacToeBoard()
{
	bReplicates = true;

	_boardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh"));
	RootComponent = _boardMesh;

	_boardRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BoardRoot"));
	_boardRoot->SetupAttachment(_boardMesh);

	_xMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("XInstancedStaticMesh"));
	_xMesh->SetupAttachment(_boardRoot);

	_oMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("OInstancedStaticMesh"));
	_oMesh->SetupAttachment(_boardRoot);

	_board.Init({3, 3});
}

void ATicTacToeBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATicTacToeBoard, _board);
}

void ATicTacToeBoard::SetTileState(const FVector& location, ECellState state)
{
	SetTileState(GetTileIndex(location), state);
}

void ATicTacToeBoard::SetTileState(const FIntVector2& index, ECellState state)
{
	if (HasAuthority())
	{
		if (_isFinished)
		{
			return;
		}

		ATicTacToeGameMode* gameMode = Cast<ATicTacToeGameMode>(UGameplayStatics::GetGameMode(this));
		if (gameMode == nullptr)
		{
			return;
		}

		if (state != gameMode->GetCurrentRole())
		{
			return;
		}

		if (_board[index.X][index.Y] != ECellState::Empty)
		{
			return;
		}

		_board[index.X][index.Y] = state;

		NetMulticast_UpdateMeshes(index, state);

		if (CheckIsFinished(index))
		{
			gameMode->FinishedGame();
		}
		else
		{
			gameMode->PlayerFinishedTurn();
		}
	}
}

void ATicTacToeBoard::NetMulticast_UpdateMeshes_Implementation(const FIntVector2& index, ECellState state)
{
	if (state == ECellState::X)
	{
		_xMesh->AddInstance(FTransform(GetTileLocation(index)));
	}
	else if (state == ECellState::O)
	{
		_oMesh->AddInstance(FTransform(GetTileLocation(index)));
	}
}

void ATicTacToeBoard::NetMulticast_ClearBoard_Implementation()
{
	_xMesh->ClearInstances();
	_oMesh->ClearInstances();
	_board.Clear();

	_isFinished = false;
}

void ATicTacToeBoard::BeginPlay()
{
	Super::BeginPlay();
}

FIntVector2 ATicTacToeBoard::GetTileIndex(const FVector& worldLocation) const
{
	const FVector localLocation = _boardRoot->GetComponentTransform().InverseTransformPosition(worldLocation);
	const FVector localLocationNormalized = localLocation / _tileSize;

	return FIntVector2(FMath::Clamp(FMath::TruncToInt(localLocationNormalized.X), 0, 2),
	                   FMath::Clamp(FMath::TruncToInt(localLocationNormalized.Z), 0, 2));
}

FVector ATicTacToeBoard::GetTileLocation(const FIntVector2& index) const
{
	return FVector(index.X * _tileSize + 0.5f * _tileSize, 0.0f, index.Y * _tileSize + 0.5f * _tileSize);
}

bool ATicTacToeBoard::CheckIsFinished(const FIntVector2& lastChangedIndex)
{
	if (_isFinished)
	{
		return true;
	}

	_isFinished = _board.CheckRowFinished(lastChangedIndex.Y) || _board.CheckColumnFinished(lastChangedIndex.X) ||
		_board.CheckLeftDiagonalFinished() || _board.CheckRightDiagonalFinished();

	return _isFinished;
}
