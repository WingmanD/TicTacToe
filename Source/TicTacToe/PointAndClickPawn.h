#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PointAndClickPawn.generated.h"

class UCameraComponent;
UCLASS()
class TICTACTOE_API APointAndClickPawn : public APawn
{
	GENERATED_BODY()

public:
	APointAndClickPawn();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCameraComponent* _camera;
};
