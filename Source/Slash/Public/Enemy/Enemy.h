

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterType.h"
#include "Enemy.generated.h"


class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()


public:
	AEnemy();

	//继承自<AActor>
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

	//接口
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;//GetHit_Implementation覆盖了GitHit，因为GitHit设置成了原生蓝图，不再是虚函数。

protected:
	//继承自<AActor>
	virtual void BeginPlay() override;


	//继承自<ABaseCharacter>
	virtual void Die() override;//播放死亡的蒙太奇
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;


	

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	
	//AI行为
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinish();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutSideCombatRadius();
	bool IsOutSideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);//判断我是否在目标范围内
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();


	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);//绑定到委托//Pawn感应组件的回调

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;//控件组件，代表血量。

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing; //感知组件

	//生成敌人的武器
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClasss;


	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;//进入战斗半径

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;//攻击半径

	//敌人控制器
	UPROPERTY()
	class AAIController* EnemyController;

	//常规的巡逻目标
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	//存储巡逻目标
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*>PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;//巡逻范围

	//定时器
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere,Category=Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;

	//死亡周期
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;

};
