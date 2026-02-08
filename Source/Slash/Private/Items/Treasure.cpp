// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include"Characters/SlashCharacter.h" 
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	//角色重叠时设置角色上的重叠物品
	if (SlashCharacter)
	{
		//在指定位置播放声音
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}

		//销毁拾取物品
		Destroy();
	}
}
