// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickupInterface.h" 


//灵魂将会被拾取并销毁
void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	//角色重叠时设置角色上的重叠物品
	if (PickupInterface)
	{
		PickupInterface->AddSouls(this);
	}
	SpanwPickupSystem();
	SpanwPickupSound();
	
	Destroy();
}
