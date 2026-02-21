// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Interfaces/PickupInterface.h" 
#include "Components/SphereComponent.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	//角色重叠时设置角色上的重叠物品
	if (PickupInterface)
	{
		PickupInterface->AddGold(this);
		//在指定位置播放声音
		SpanwPickupSound();

		//销毁拾取物品
		Destroy();
	}

}
