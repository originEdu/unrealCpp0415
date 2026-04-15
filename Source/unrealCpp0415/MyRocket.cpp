// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRocket.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

//파티클 필수 헤더
#include "Particles/ParticleSystem.h"           
#include "Particles/ParticleSystemComponent.h" 

// Sets default values
AMyRocket::AMyRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Box 컴포넌트 생성
	//루트로 선택 후 박스 크기 조절
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(32.0f, 8.0f, 8.0f));
	
	//로켓 staticMesh 생성 후 박스 자식으로
	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	Rocket->SetupAttachment(Box);
	//방향 회전
	Rocket->SetRelativeRotation(FRotator(-90.0f, 0, 0));
	//Mesh적용
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("/Script/Engine.StaticMesh'/Game/Rocket/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Rocket.Succeeded())
	{
		Rocket->SetStaticMesh(SM_Rocket.Object);
	}

	//ProgectileMovement컴포넌트 생성 및 초기 속도 및 중력 설정
	ProgectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProgectileMovement"));
	ProgectileMovement->InitialSpeed = 1000.0f;
	ProgectileMovement->MaxSpeed = 1000.0f;
	ProgectileMovement->ProjectileGravityScale = 0.0f;
	ProgectileMovement->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AMyRocket::BeginPlay()
{
	Super::BeginPlay();
	
	//생명주기 설정
	SetLifeSpan(3.0f);

	//BeginOverlap 
	OnActorBeginOverlap.AddDynamic(this, &AMyRocket::ProcessActorBeginOverlap);
}

// Called every frame
void AMyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

//BeginOverlap 후 삭제
void AMyRocket::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessActorBeginOverlap 실행됨"));
	
	Destroy();
}

