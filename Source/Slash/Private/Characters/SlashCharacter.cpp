// Fill out your copyright notice in the Description page of Project Settings.//test


#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"//弹簧臂头文件
#include "Camera/CameraComponent.h"//摄像机头文件
#include "GameFramework/CharacterMovementComponent.h"//人物移动头文件
#include "GroomComponent.h"//毛发头文件
#include"Items/Item.h"
#include"Items/Weapons/Weapon.h"
#include"Animation/AnimMontage.h"


ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//用控制器旋转，让人物视角转向正常，人体不会随着“超人飞行”，而是站着换视角。
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate=FRotator(0.f, 400.f, 0.f);

	//创造摇臂，附加到根组件上
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;//设置臂长，由于设立了蓝图可见，所以蓝图也可以改。这个相当于默认值。

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);//设置到CameraBoom目录里面

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh()); //连接到网络
	Hair->AttachmentName = FString("head");//指定一个插槽，可以在上面放东西，可以接上头发

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));//每个 CreateDefaultSubobject 的名字必须是唯一的。否则会崩溃。
	Eyebrows->SetupAttachment(GetMesh()); //连接到网络
	Eyebrows->AttachmentName = FString("head");//指定一个插槽，可以在上面放东西，可以接上眉毛
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//和鸟类似，绑定轴映射
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);

	//绑定跳跃//Jump本身在角色类就存在，在编辑器里设置了。而且我们没有改写它，它不用在SlashCharacter里设置。IE_Pressed包含枚举常量，比如：按下。
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	//绑定装备
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	//绑定攻击
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("SlashCharacter"));//给主角添加标签，便于识别
}


void ASlashCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return; //有动作过程时不移动。
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return; //有动作过程时不移动。
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ASlashCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

//按下E键后~
void ASlashCharacter::EKeyPressed()
{
	//识别武器
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else //判断是否有武器，决定是否播放装备武器的动画
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

//攻击函数
void ASlashCharacter::Attack()
{
	Super::Attack();
	
	if (CanAttack()) //攻击状态判断
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	
}

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon; //角色状态，放映我们是否装备了武器
	OverlappingItem = nullptr; //置空重叠物品
	EquippedWeapon = Weapon; //用一个变量存储已装备的武器
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}


bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped ;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

//可以调用这个函数来播放装备蒙太奇，并传入装备（物品/武器）或卸下（物品/武器）的部分名称
void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//从网络体中获取动画实例
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}






