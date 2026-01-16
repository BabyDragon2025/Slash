// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class UCapsuleComponent;//提前声明胶囊体
class USkeletalMeshComponent;//提前声明骨骼网络组件
class USpringArmComponent;//提前声明弹簧臂组件
class UCameraComponent;//提前声明摄像机组件

UCLASS()
class SLASH_API ABird : public APawn  //Pawn派生自Actor，ABird是Actor的子类，编辑器会帮我们自动去掉A。
{
	GENERATED_BODY()

public:
	
	ABird();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input//设置玩家输入组件函数。
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);//新建一个函数，作为回调函数，处理输入绑定。
	void Turn(float Value);//准备一个鼠标控制左右视角函数
	void LookUp(float Value);//准备一个鼠标控制上下视角函数
private:
	UPROPERTY(VisibleAnywhere)//把胶囊体暴露给蓝图。
	UCapsuleComponent* Capsule;//建立胶囊体指针。准备设置胶囊体。

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;//建立骨骼网络类型指针，小鸟的。

	//添加弹簧臂组件
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	//添加摄像机组件
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
};
