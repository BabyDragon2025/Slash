// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include"Slash/DebugMacros.h"
#include"Components/SphereComponent.h" //球形组件的头文件
#include"Characters/SlashCharacter.h" // 有了它，可以在OnSphereOverlap中将其转换为SlashCharacter
#include "NiagaraComponent.h" //该头文件记得加上Niagara模块才可以使用


// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere")); //创建球形组件
	Sphere->SetupAttachment(GetRootComponent()); 

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));////创建构造球形组件
	EmbersEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	//找到我们的物体，绑定回调到委托
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap); 

	//获取我们的球体，访问它的组件结束重叠事件
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
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
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	//角色重叠时设置角色上的重叠物品
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	//角色离开时设置角色上的重叠物品
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(nullptr);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}

}

