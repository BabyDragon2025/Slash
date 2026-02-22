// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"//我们这个武器要使用到角色的内容，所以包含一下它的头文件，这样也可以直接访问public文件夹的所有内容，不用包括整个个文件夹
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"


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
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;//物品状态设置为：已装备
	//设置：装备这把武器后，它的所有者和发起者就会自动设置为持有它的角色
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName); //场景组件作为父级，TransformRules作为规则，附加对象InSocketName
	DisableSphereCollision();	
	PlayEquipSound();

	DeactivateEmbers();
}

void AWeapon::DeactivateEmbers()
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();//装备武器后，停用Niagara
	}
}

void AWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::PlayEquipSound()
{
	if (EquipSound) //在指定位置播放声音
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());//传入对象，使用的音效，对象的位置
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}



//盒式追踪
void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//敌人之间无伤害
	if (ActorIsSameType(OtherActor)) return;
	
	FHitResult BoxHit;
	BoxTrace(BoxHit);
	
	////新增部分
	//AActor* HitActor = BoxHit.GetActor();
	//if (HitActor)
	//{
	//	// 【关键修复】这里必须再次检查 HitActor！

	//	// 1. 绝对不能打自己
	//	if (HitActor == GetOwner())
	//	{
	//		return;
	//	}

	//	// 2. 再次检查阵营（因为 Trace 可能扫到了另一个没触发 Overlap 的敌人）
	//	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && HitActor->ActorHasTag(TEXT("Enemy")))
	//	{
	//		return;
	//	}
	//}
	////新增部分


	if (BoxHit.GetActor())//返回被击中的对象
	{
		if (ActorIsSameType(BoxHit.GetActor())) return;

		UGameplayStatics::ApplyDamage(BoxHit.GetActor(),Damage,GetInstigator()->GetController(),this,UDamageType::StaticClass());
		ExecuteGetHit(BoxHit);
		CreateFields(BoxHit.ImpactPoint);
	}
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());//把击中的对象转换成击中接口
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint,GetOwner());//执行这个事件，确保蓝图被触发
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*>ActorsToIgnore;
	ActorsToIgnore.Add(this);//确保我们不会击中武器本身
	ActorsToIgnore.Add(GetOwner());//确保武器不会击中使用者本身

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());//知道武器命中哪个角色，AddUnique则可以不重复添加角色。
}
