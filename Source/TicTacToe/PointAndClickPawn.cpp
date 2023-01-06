#include "PointAndClickPawn.h"

#include "Camera/CameraComponent.h"

APointAndClickPawn::APointAndClickPawn()
{
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = _camera;
}
