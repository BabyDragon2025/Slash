// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include"CharacterType.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;//提前声明弹簧臂组件
class UCameraComponent;//提前声明摄像机组件
class UGroomComponent;//提前声明毛发组件
class AItem; 
class UAnimMontage;


UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	
	ASlashCharacter();
	//virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;//GetHit_Implementation覆盖了GitHit，因为GitHit设置成了原生蓝图，不再是虚函数。


protected:
	virtual void BeginPlay() override;

	//输入的回调函数
	void MoveForward(float Value); //前进函数。
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed(); //这是一个动作映射（装备武器），因为是一次性，所以没有参数。
	virtual void Attack() override; //攻击的回调函数，用来绑定攻击键。


	//战斗有关
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override; //攻击结束通知
	virtual bool CanAttack() override;// 是否可以攻击
	bool CanDisarm();//检查卸下播放装备武器的蒙太奇动画的条件
	bool CanArm();//检查装备武器的蒙太奇动画的条件
	void Disarm();
	void Arm();
	void PlayEquipMontage(const FName& SectionName);//装备动画的函数

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();//武器附着到脊椎上

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();//武器从背上拿出的动画，也就是这次的附着点在右手

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();//装备动画结束时

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	//Character的对应组件

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;//添加弹簧臂组件

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;//添加摄像机组件
	
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;//添加毛发组件（头发）

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;////添加毛发组件（眉毛）

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;  //创建一个重叠的游戏物品，需要在物品重叠时，才能从物品类里设置这个属性

	

	
	//装备武器的蒙太奇动画
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;


	ECharacterState CharacterState = ECharacterState::ECS_Unequipped; //使用枚举常量来控制游戏逻辑

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied; //在攻击过程中阻止继续攻击


public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; } //有了OverlappingItem的公开设置器。小型的的函数设置成内联更高效
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; } //角色状态的公开访问器

};
