// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyEnhancedPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class UNREALCPP0415_API AMyEnhancedPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyEnhancedPlayerController();
	virtual void OnPossess(APawn* aPawn) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;
};
