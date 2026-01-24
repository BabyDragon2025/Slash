// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"  //要使用SlashCharacter，就要引入它的头文件和类路径。
#include "GameFramework/CharacterMovementComponent.h"//移动组件
#include "Kismet/KismetMathLibrary.h"//使用Kismet数学库的静态函数 

void USlashAnimInstance::NativeInitializeAnimation()
{
	
	Super::NativeInitializeAnimation(); //因为是重写函数，所以调用父版本
	
	//TryGetPawnOwner();//返回pawn。进行类型转换
	SlashCharacter=Cast<ASlashCharacter>(TryGetPawnOwner());//获取tryGetPawnOwner()的返回值，尝试将其强转为SlashCharacter。这样初始化SlashCharacter
	if (SlashCharacter)
	{
		//用这个返回的角色移动组件来初始化我们的GetCharacterMovement变量。
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	//UKismetMathLibrary这个类的函数都是静态的，不用创建Kismet数学库类型的对象，这些函数不改变任何状态和结果，只是计算和返回结果
	//检查这个指针，确保只在非空时访问
	if (SlashCharacterMovement)
	{
		//获取SlashCharacterMovement在xy方向上的速度
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);

		//调用IsFalling()来设置跳跃变量。
		IsFalling = SlashCharacterMovement->IsFalling();

		CharacterState = SlashCharacter->GetCharacterState(); //时刻更新角色状态
	}
}
