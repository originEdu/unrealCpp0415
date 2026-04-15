// Fill out your copyright notice in the Description page of Project Settings.


#include "MyP38.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "MyRocket.h"
// Sets default values
AMyP38::AMyP38()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Box컴포넌트 생성
	//루트로 선택 후 박스 크기 조절
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(42.0f, 32.0f, 16.0f));
	//미사일과 충돌 때문에 일단은 overlap 해제
	Box->SetGenerateOverlapEvents(false);

	//Body 생성 후 Box 자식으로 설정
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	//Body에 Mesh 적용
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	//프로펠러 생성 후 Body 자식으로 설정
	LeftPropeller = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPropeller"));
	LeftPropeller->SetupAttachment(Body);
	RightPropeller = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPropeller"));
	RightPropeller->SetupAttachment(Body);

	//프로펠러에 Mesh적용
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		LeftPropeller->SetStaticMesh(SM_Propeller.Object);
		RightPropeller->SetStaticMesh(SM_Propeller.Object);
	}

	//프로펠러 상대좌표로 위치 조정
	LeftPropeller->SetRelativeLocation(FVector(37.0f, 21.0f, 1.0f));
	RightPropeller->SetRelativeLocation(FVector(37.0f, -21.0f, 1.0f));

	//Arrow 생성 및 위치 조정
	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrow"));
	LeftArrow->SetupAttachment(Body);
	LeftArrow->SetRelativeLocation(FVector(47.0f, -21.0f, -12.0f));
	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrow"));
	RightArrow->SetupAttachment(Body);
	RightArrow->SetRelativeLocation(FVector(47.0f, 21.0f, -12.0f));

	//스프링암 생성 및 초기 설정 세팅
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 30.0f);
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagMaxDistance = 10.0f;
	SpringArm->bDoCollisionTest = false;

	//카메라 생성 및 스프링암 자식으로 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//FloatingPawnMovement컴포넌트 생성 후 MaxSpeed 세팅
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = MoveSpeed;
}

// Called when the game starts or when spawned
void AMyP38::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyP38::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//전방이동
	AddMovementInput(GetActorForwardVector(), BoostValue);

	//프로펠러 회전
	RotatePorpller(LeftPropeller);
	RotatePorpller(RightPropeller);
}

//프로펠러 회전 함수
void AMyP38::RotatePorpller(UStaticMeshComponent* propeller)
{
	propeller->AddLocalRotation(FRotator(0, 0, PropellerRotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

// Called to bind functionality to input
void AMyP38::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//블루프린트로 구현한 EnhancedInput 때문에 잠시 주석처리
	//PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyP38::Pitch);
	//PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyP38::Roll);
	//PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyP38::Fire);
	//PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Pressed, this, &AMyP38::Boost);
	//PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Released, this, &AMyP38::UnBoost);
}

void AMyP38::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(60.0f * Value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0, 0));
}

void AMyP38::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0,0,60.0f * Value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

void AMyP38::Fire()
{
	//Flip Flop 구현
	//로켓 스폰 위치 설정
	FTransform WorldTransform = 
		bNextFireIsLeft ? LeftArrow->GetComponentTransform() : RightArrow->GetComponentTransform();
	//로켓 스폰 옵션 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//로켓 스폰
	GetWorld()->SpawnActor<AMyRocket>(AMyRocket::StaticClass(), WorldTransform, SpawnParams);
	//플립플롭 변수 반전
	bNextFireIsLeft = !bNextFireIsLeft;

}

void AMyP38::Boost()
{
	BoostValue = 1.0f;
}

void AMyP38::UnBoost()
{
	BoostValue = 0.5f;
}

