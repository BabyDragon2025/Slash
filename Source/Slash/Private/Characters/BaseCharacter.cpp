// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include"Components/BoxComponent.h"
#include"Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{

}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//从网络体中获取动画实例
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
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

	
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
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

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes )
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	//播放被击中的声音
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}



void ABaseCharacter::PlayAttackMontage()
{

}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();//关闭武器碰撞时(攻击结束后)清空命中的对象
	}
}


