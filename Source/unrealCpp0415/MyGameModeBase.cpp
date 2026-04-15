// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyP38.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyP38::StaticClass(); // 디폴트폰클래스 변경
	PlayerControllerClass = AMyPlayerController::StaticClass(); // 플레이어 컨트롤러 변경
}
