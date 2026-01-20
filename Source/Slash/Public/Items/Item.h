// Fill out your copyright notice in the Description page of Project Settings.

#pragma once//避免重复包含头文件

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"//因为AActor继承与Actor，要使用Actor内容要记得包含它的头文件。
#include "Item.generated.h"

class USphereComponent; //声明一个球形组件

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()//理解为一个功能增强的普通C++类，它能参与虚幻引擎的幕后工作，从而与ue的反射系统连接。作用：可以基于这个类创建蓝图，把属性暴露给蓝图等等。
	
public:	
	
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Sine Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	//蓝图里添加正弦函数组件
	UFUNCTION(BlueprintPure)
	float TransformedSin();

	//蓝图里添加余弦函数组件
	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template<typename T>
	T Avg(T First, T Second);

	//来自PrimitiveComponent.h,搜索OverlappedComponent
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	float RunningTime; 

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere; //声明一个USphereComponent类的指针
};

//内联函数：通过在调用处直接嵌入函数体代码来替代函数调用，以牺牲少量代码体积为代价，消除函数调用的开销从而提升程序执行效率。
//可以消除函数调用开销，提升执行效率。

//模板函数
template<typename T>
inline T AItem::Avg(T First, T Second)  
{
	return (First + Second) / 2;
}
