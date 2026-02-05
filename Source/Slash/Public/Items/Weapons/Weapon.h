// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"

#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent; //盒子组件

/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();//武器的构造函数，初始化武器

	void Equip(USceneComponent* InParent,FName InSocketName);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);//自定义一个函数来处理装备的武器//传入角色和插槽名字
	
	TArray<AActor*>IgnoreActors;//记录被击中的角色
protected:
	virtual void BeginPlay() override;
	
	//由于是重写函数，所以不需要UFuntion来暴露给蓝图，因为原本的函数就有了，可以继承过来。多写了会报错。记得加上override标记为重写函数
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//碰撞箱重叠
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	
	UPROPERTY(EditAnywhere,Category="Weapon Properties")
	USoundBase* EquipSound; //建立武器声音变量。

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;//武器的盒子组件
	//添加场景组件，用他们代表碰撞体的起点和终点
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
