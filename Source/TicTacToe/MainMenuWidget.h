﻿#pragma once

#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UEditableTextBox;
class UButton;

UCLASS()
class TICTACTOE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* _hostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* _joinButton;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* _ipAddressTextBox;

	UPROPERTY(meta = (BindWidget))
	UButton* _quitButton;

private:
	UFUNCTION()
	void Host();

	UFUNCTION()
	void Join();

	UFUNCTION()
	void QuitGame();

};
