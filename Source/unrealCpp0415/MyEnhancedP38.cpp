// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnhancedP38.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyRocket.h"
// Sets default values
AMyEnhancedP38::AMyEnhancedP38()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Left->SetRelativeLocation(FVector(39.4f, -20.0f, 1.4f));

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);
	Right->SetRelativeLocation(FVector(39.4f, 20.0f, 1.4f));


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	static ConstructorHelpers::FClassFinder<AMyRocket> BP_Rocket(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/CPP/BP_Rocket.BP_Rocket_C'"));
	if (BP_Rocket.Succeeded())
	{
		RocketTemplate = BP_Rocket.Class;
	}

}

// Called when the game starts or when spawned
void AMyEnhancedP38::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyEnhancedP38::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), BoostValue);

	Left->AddRelativeRotation(FRotator(0, 0, DeltaTime * 3600.0f));
	Right->AddRelativeRotation(FRotator(0, 0, DeltaTime * 3600.0f));

}

// Called to bind functionality to input
void AMyEnhancedP38::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Rotate, ETriggerEvent::Triggered, this, &AMyEnhancedP38::Rotate);
		UIC->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AMyEnhancedP38::Fire);
		UIC->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AMyEnhancedP38::Boost);
		UIC->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AMyEnhancedP38::Unboost);
	}

}

void AMyEnhancedP38::Rotate(const FInputActionValue& Value)
{
	FVector2D Rot = Value.Get<FVector2D>();
	Rot = Rot * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60.0f;

	AddActorLocalRotation(FRotator(Rot.Y, 0, Rot.X));
}

void AMyEnhancedP38::Fire(const FInputActionValue& Value)
{
	GetWorld()->SpawnActor<AMyRocket>(RocketTemplate, Arrow->K2_GetComponentToWorld());
}

void AMyEnhancedP38::Boost(const FInputActionValue& Value)
{
	BoostValue = 1.0f;
}

void AMyEnhancedP38::Unboost(const FInputActionValue& Value)
{
	BoostValue = 0.5f;
}

