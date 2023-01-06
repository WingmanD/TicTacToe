#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (_hostButton != nullptr)
	{
		_hostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::Host);
	}

	if (_joinButton != nullptr)
	{
		_joinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::Join);
	}

	if (_quitButton != nullptr)
	{
		_quitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::Host()
{
	UGameplayStatics::OpenLevel(this, "TicTacToeMap", true, "listen");
}

void UMainMenuWidget::Join()
{
	if (_ipAddressTextBox == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UMainMenuWidget::Join: IP Address Text Box is null"));
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UMainMenuWidget::Join: Player Controller is null"));
		return;
	}

	PlayerController->ClientTravel(_ipAddressTextBox->GetText().ToString(), TRAVEL_Absolute);
}

void UMainMenuWidget::QuitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}
