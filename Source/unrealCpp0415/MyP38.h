// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyP38.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class UMyStaticMeshComponent;

UCLASS()
class UNREALCPP0415_API AMyP38 : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyP38();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//프로펠러 회전 함수
	void RotatePorpller(UStaticMeshComponent* propeller);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//컴포넌트 세팅
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> Body;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> LeftPropeller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> RightPropeller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UArrowComponent> LeftArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UArrowComponent> RightArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	//Input Key funtion
	void Pitch(float Value);
	void Roll(float Value);
	void Fire();
	void Boost();
	void UnBoost();

	//변수들 세팅
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float MoveSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float PropellerRotationSpeed = 3600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float RotationSpeed = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
	float BoostValue = 0.5f;
	//플립플롭을 위한 변수
	bool bNextFireIsLeft = true;
};
