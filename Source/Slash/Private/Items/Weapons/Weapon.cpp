// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"//我们这个武器要使用到角色的内容，所以包含一下它的头文件，这样也可以直接访问public文件夹的所有内容，不用包括整个个文件夹
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);//碰撞启用类型，对应蓝图。
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//设置单个通道。

	BoxTraceStart = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trace Start"));
	BoxTraceStart -> SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);//发生重叠事件时收到回调。OnBoxOverlap作为回调函数
}

//处理物品附加操作的函数
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)  
{
	AttachMeshToSocket(InParent, InSocketName); //场景组件作为父级，TransformRules作为规则，附加对象InSocketName
	ItemState = EItemState::EIS_Equipped;//物品状态设置为：已装备
	if (EquipSound) //在指定位置播放声音
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());//传入对象，使用的音效，对象的位置
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
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

//盒式追踪
void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*>ActorsToIgnore;
	ActorsToIgnore.Add(this);//确保我们不会击中武器本身

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true
	);
	if (BoxHit.GetActor())//返回被击中的对象
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());//把击中的对象转换成击中接口
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());//知道武器命中哪个角色，AddUnique则可以不重复添加角色。
	}
}
