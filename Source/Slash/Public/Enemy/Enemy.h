

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"//让敌人可以继承接口里的函数和类
#include "Characters/CharacterType.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ACharacter,public IHitInterface //继承两个父类
{
	GENERATED_BODY()

public:
	
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;//GetHit_Implementation覆盖了GitHit，因为GitHit设置成了原生蓝图，不再是虚函数。
	void DirectionalHitReact(const FVector& ImpactPoint);
	//造成伤害
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
private:

	//组件(Component)



	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;//自定义组件：属性

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;//控件组件，代表血量。

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing; //感知组件

	//动画蒙太奇，这里保留蒙太奇变量，未来可能添加更多
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;
	//死亡蒙太奇变量
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound; //该变量存储MetaSound资产类型的值

	//击中的效果粒子，可以是血液，小石头等等
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	UPROPERTY()
	AActor* CombatTarget;//战斗目标

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;//进入战斗半径

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;//攻击半径

	//导航

	//敌人控制器
	UPROPERTY()
	class AAIController* EnemyController;

	//常规的巡逻目标
	UPROPERTY(EditInstanceOnly,Category="AI Navigation")
	AActor* PatrolTarget;

	//存储巡逻目标
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*>PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;//巡逻范围

	//定时器
	FTimerHandle PatrolTimer;
	void PatrolTimerFinish();

	UPROPERTY(EditAnywhere,Category="AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

protected:
	
	virtual void BeginPlay() override;

	void Die();//播放死亡的蒙太奇
	//判断我是否在目标范围内
	bool InTargetRange(AActor* Target,double Radius);

	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);//绑定到委托

	//播放蒙太奇函数
	void PlayHitReactMontage(const FName& SectionName);
	

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
};
