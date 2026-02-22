// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//构造函数
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);
protected:
	virtual void BeginPlay() override;


private:
	//当前生命值
	UPROPERTY(EditAnywhere,Category="Actor Attributes")
	float Health;
	//最大生命值
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	//当前体力值
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;
	//最大体力值
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	//最大体力值
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 14.f;
	//每秒恢复体力
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 8.f;
public:
	//处理健康变量的变化
	void ReceiveDamage(float Damage);

	void UseStamina(float StaminaCost);
	//获取健康值百分比
	float GetHealthPercent();

	float GetStaminaPercent();
	//判断是否活着
	bool IsAlive();

	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);

	//为金币和灵魂设置公共获取器
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }

	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
};
