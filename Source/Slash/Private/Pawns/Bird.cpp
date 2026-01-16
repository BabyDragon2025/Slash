// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"// 胶囊体头文件
#include "Components/SkeletalMeshComponent.h"//骨骼网络体头文件
#include "GameFramework/SpringArmComponent.h"//弹簧臂头文件
#include "Camera/CameraComponent.h"//摄像机头文件

ABird::ABird()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));//创建胶囊体并且命名
	Capsule->SetCapsuleHalfHeight(20.f);//设置胶囊体高度
	Capsule->SetCapsuleRadius(15.f);//设置胶囊体半径
	SetRootComponent(Capsule);// 相当于RootComponent = Capsule;修改默认根组件。

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	BirdMesh->SetupAttachment(GetRootComponent());//把鸟的模型附加到根组件————胶囊体上。

	//改了SpringArm->CamraBoom,来解决蓝图组件空白bug。
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));//设置弹簧体组件。
	CameraBoom->SetupAttachment(GetRootComponent()); //组件附加到根目录下。
	CameraBoom->TargetArmLength = 300.f;//设置弹簧臂长度。

	//与弹簧臂类似，但是文件设置在弹簧臂目录下
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	AutoPossessPlayer = EAutoReceiveInput::Player0;//EAutoReceiveInput自动控制玩家的类型，这里控制玩家0，也就是我们自己。
}


void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABird::MoveForward(float Value)
{
	//Pawn类继承了一个Controller变量，检查空指针。
	if (Controller && (Value != 0.f))
	{
		FVector Forward = GetActorForwardVector();// 建立前进向量
		//AddMovementInput()就是获取这个移动方向，然后交给移动组件处理
		AddMovementInput(Forward, Value);//使用该前进向量，Value不会影响前进向量，该函数只在乎0、正数、负数。
	}
}

void ABird::Turn(float Value)
{
	AddControllerYawInput(Value);//调用左右转向函数
}

void ABird::LookUp(float Value)
{
	AddControllerPitchInput(Value);//调用上下转向函数
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//前进函数绑定到前进轴的映射上，参数为：轴映射名字设为MoveForward，this代表该鸟对象，回调函数地址）
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);

	//把鼠标的两个操作绑定到轴映射上
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);
}

