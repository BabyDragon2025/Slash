// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"//我们这个武器要使用到角色的内容，所以包含一下它的头文件，这样也可以直接访问public文件夹的所有内容，不用包括整个个文件夹

//重写了重叠信息
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//要调用父类版本，就要调用该函数的父类方法
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor); //把对象类型强转为ASlashCharacter，返回给变量
	if (SlashCharacter)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true);//创建一个FAttachmentTransformRules结构体
		//武器连接到SlashCharacter角色的rightHandSocket插槽上
		ItemMesh->AttachToComponent(SlashCharacter->GetMesh(), TransformRules, FName("RightHandSocket"));
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
