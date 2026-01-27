// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"//我们这个武器要使用到角色的内容，所以包含一下它的头文件，这样也可以直接访问public文件夹的所有内容，不用包括整个个文件夹

//处理物品附加操作的函数
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)  
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);  //场景组件作为父级，TransformRules作为规则，附加对象InSocketName
	ItemState = EItemState::EIS_Equipped;//物品状态设置为：已装备
}

//重写了重叠信息
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//要调用父类版本，就要调用该函数的父类方法
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
