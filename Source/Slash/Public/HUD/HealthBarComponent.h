// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:

	void SetHealthPercent(float Percent);//设置百分比
private:
	UPROPERTY()
	class UHealthBar* HealthBarWidget;//防止频繁调用函数过程中多次进行强制类型转换
};
