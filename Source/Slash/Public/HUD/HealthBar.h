// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	//让健康条的C++变量与蓝图变量连接起来，注意名字一致
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthBar;
};
