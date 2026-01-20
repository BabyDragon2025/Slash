// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include"Slash/DebugMacros.h"
#include"Components/SphereComponent.h" //球形组件的头文件



// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere")); //创建球形组件
	Sphere->SetupAttachment(GetRootComponent()); 
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	//找到我们的物体，绑定回调到委托
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap); 
}

float AItem::TransformedSin( )
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();//获取角色名称
	//把它打印到屏幕上
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

}

