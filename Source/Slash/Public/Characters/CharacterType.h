#pragma once

//我们的枚举类加E前缀，方便区分
//创建一个枚举类。使用这种类型的枚举常量时，我们会完全限定它，防止多个枚举可能具有相同的枚举常量//用这个表示状态比用整数更好
UENUM(BlueprintType) //把枚举暴露给蓝图用作类型,否则里面的变量不是蓝图变量，蓝图无法识别和使用
enum class ECharacterState :uint8 //uint8可以优化一下枚举，使其每个常量都是8位无符号整数
{
	ECS_Unequipped UMETA(DisplayName = "unequipped"), //把常量显示名称设为unequipped
	ECS_EquippedOneHandedWeapon  UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon  UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"), //此状态：阻止角色做当前活动以外的其它动作
	EAS_Attacking UMETA(DisplayName="Attacking"),//此状态：攻击时的姿态
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")//装备进行的过程
};

//存活与死亡状态
UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6"),

	EDP_MAX UMETA(DisplayName = "DefaultMax")
};

//敌人行为状态//ps：枚举变量有默认值。第一个是0，后面1、2、3...
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"), 
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),//处于战斗之中
	EES_NoState UMETA(DisplayName = "NoState")
};