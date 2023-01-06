#pragma once

#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;

UCLASS()
class TICTACTOE_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* _resumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* _quitButton;
	
private:
	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void QuitGame();
};
