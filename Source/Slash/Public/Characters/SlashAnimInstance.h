// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlashAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;  //重写函数，继承自animinstance类。重写蓝图事件类表的函数。
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;//创建一个变量来初始化变量。

	UPROPERTY(BlueprintReadOnly,Category = Movement)
	class UCharacterMovementComponent* SlashCharacterMovement;//创建移动组件

	//创建地面速度，归类到移动
	UPROPERTY(BlueprintReadOnly,Category=Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;//判断是否跳跃。每帧都要判断，所以放到NativeUpdateAnimation
};
