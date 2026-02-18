// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;


UCLASS()
class SLASH_API ABaseCharacter : public ACharacter,public IHitInterface
{
	GENERATED_BODY()

public:
	
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void Attack();
	virtual void Die();//播放死亡的蒙太奇
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	virtual bool CanAttack();// 是否可以攻击
	bool IsAlive();
	void PlayHitReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd(); //攻击结束通知


	//设置碰撞开关。
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);


	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;//表示当前持有的武器

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);//可自动播放对应的蒙太奇动画.
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	//该变量存储MetaSound资产类型的值
	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound; 

	//击中的效果粒子，可以是血液，小石头等等
	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticles;

	//动画蒙太奇，这里保留蒙太奇变量，未来可能添加更多
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage;

	//动画蒙太奇，这里保留蒙太奇变量，未来可能添加更多
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

	//死亡蒙太奇变量
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;
};
