#pragma once

//我们的枚举类加E前缀，方便区分
//创建一个枚举类。使用这种类型的枚举常量时，我们会完全限定它，防止多个枚举可能具有相同的枚举常量//用这个表示状态比用整数更好
UENUM(BlueprintType) //把枚举暴露给蓝图用作类型
enum class ECharacterState :uint8 //uint8可以优化一下枚举，使其每个常量都是8位无符号整数
{
	ECS_Unequipped UMETA(DisplayName = "unequipped"), //把常量显示名称设为unequipped
	ECS_EquippedOneHandedWeapon  UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon  UMETA(DisplayName = "Equipped Two-Handed Weapon")
};
