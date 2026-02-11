// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include"Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"



AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//设置碰撞预设
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//设置是否生成重叠事件
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);//防止胶囊体阻挡相机
	
	////构造敌人的属性
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//从网络体中获取动画实例
	//播放死亡蒙太奇动画
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 5); 
		FName SectionName = FName(); //初始化一下名字变量
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5:
			SectionName = FName("Death6");
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	//死亡后禁用胶囊体碰撞 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//从网络体中获取动画实例
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint,FColor::Orange);//测试该函数。

	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		//播放死亡动画
		Die();
	}

	

	//播放被击中的声音
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
	//设置击中效果。对象的世界、种类、位置。  //这里的函数内部的GetWorld()用this也可以
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();//敌人的前向向量。GetActorForwardVector()返回的 Forward 向量确实是单位向量（模长为 1），这是虚幻引擎的默认设计

	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);//击中点的Z向量与敌人的Z方向向量一致，让调试箭头和角度与地面平行
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();//敌人的击中向量。用GetSafeNormal()来把向量结果归一化变成单位向量

	//Forward*ToHit = |Forward||ToHit|*cos(theta)
	//|Forward|=|ToHit|=1,所以,Forward*ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//余弦角度再取反函数可以得到角度
	double Theta = FMath::Acos(CosTheta);
	//把弧度转换成度数，这样弧度单位转换成度的单位
	Theta = FMath::RadiansToDegrees(Theta);

	//如果叉积方向向下，代表右边被击中，角度为负数
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	//根据角度选择播放的动画
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);//播放左边受击动画

	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100, 5.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta:%f"), Theta));
	}
	//绘制调试箭头
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());//时刻更新我们的健康值
		
	}
	return DamageAmount;
}

