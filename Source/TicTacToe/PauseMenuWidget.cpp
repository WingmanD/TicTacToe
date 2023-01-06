#include "PauseMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (_resumeButton != nullptr)
	{
		_resumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (_quitButton != nullptr)
	{
		_quitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitGame);
	}
}

void UPauseMenuWidget::ResumeGame()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UPauseMenuWidget::QuitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}
