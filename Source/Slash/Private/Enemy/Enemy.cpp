// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"//人物移动头文件
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "navigation/PathFollowingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Slash/DebugMacros.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//设置碰撞预设
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//设置是否生成重叠事件
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);//防止胶囊体阻挡相机
	
	////构造敌人的属性
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	//设置敌人ai寻路时的转向逻辑
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//设置感知组件
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

}


void AEnemy::PatrolTimerFinish()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//设置血条的显示逻辑。
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);//一开始不显示
	}

	EnemyController = Cast <AAIController>(GetController());
	//控制敌人巡逻移动
	MoveToTarget(PatrolTarget);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);//绑定
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//从网络体中获取动画实例
	//播放死亡蒙太奇动画
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 5); 
		FName SectionName = FName(); //初始化一下名字变量
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5:
			SectionName = FName("Death6");
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	////死亡后隐藏血条
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	//死亡后禁用胶囊体碰撞 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//死亡后消失
	SetLifeSpan(5.f);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();//得到敌人与目标的距离
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_SPHERE_SingleFrame(Target->GetActorLocation());

	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	//防止重复一个巡逻点
	TArray<AActor*>ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

//设置发现目标后的逻辑
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) return;//确保只调用一次MoveToTarget
	if (SeenPawn->ActorHasTag(FName("SlashCharacter")))
	{	
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;

		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
			UE_LOG(LogTemp, Warning, TEXT("Pawn Seen , Chase Palyer"));
		}
	}
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();//从网络体中获取动画实例
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (EnemyState > EEnemyState::EES_Patrolling) 
	{
		//检查战斗目标
		CheckCombatTarget();
	}
	else
	{
		//检查巡逻目标
		CheckPatrolTarget();
	}

}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		//设置定时器
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinish, WaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		//战斗半径以外，失去兴趣
		CombatTarget = nullptr;//超出攻击范围则不设置战斗目标
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);//超出范围隐藏血条
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
		UE_LOG(LogTemp, Warning, TEXT("Lose Interest"));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		//攻击范围之外，追赶主角
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
		UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		//攻击范围以内，攻击主角
		EnemyState = EEnemyState::EES_Attacking;
		//播放攻击动画
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}
}


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint,FColor::Orange);//测试该函数。

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);//第一次命中显示血条。
	}
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		//播放死亡动画
		Die();
	}

	

	//播放被击中的声音
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
	//设置击中效果。对象的世界、种类、位置。  //这里的函数内部的GetWorld()用this也可以
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();//敌人的前向向量。GetActorForwardVector()返回的 Forward 向量确实是单位向量（模长为 1），这是虚幻引擎的默认设计

	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);//击中点的Z向量与敌人的Z方向向量一致，让调试箭头和角度与地面平行
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();//敌人的击中向量。用GetSafeNormal()来把向量结果归一化变成单位向量

	//Forward*ToHit = |Forward||ToHit|*cos(theta)
	//|Forward|=|ToHit|=1,所以,Forward*ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//余弦角度再取反函数可以得到角度
	double Theta = FMath::Acos(CosTheta);
	//把弧度转换成度数，这样弧度单位转换成度的单位
	Theta = FMath::RadiansToDegrees(Theta);

	//如果叉积方向向下，代表右边被击中，角度为负数
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	//根据角度选择播放的动画
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);//播放左边受击动画

	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100, 5.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta:%f"), Theta));
	}
	//绘制调试箭头
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());//时刻更新我们的健康值
		
	}
	CombatTarget = EventInstigator->GetPawn();//如果敌人收到伤害，就会设置战斗目标
	return DamageAmount;
}

