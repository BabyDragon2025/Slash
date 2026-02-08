// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor,public IHitInterface
{
	GENERATED_BODY()
	
public:	
	
	ABreakableActor();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;//几何集合组件

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;//胶囊体，确保物品碎后碰撞关闭。
private:	

	//TSubclassOf是一个包装器，把想要的类包装在里面,减少选择错误类的可能。所以不用原始的UClass
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TSubclassOf<class ATreasure> TreasureClass; //UClass使生成的角色是蓝图，而不是原始的C++类
};
