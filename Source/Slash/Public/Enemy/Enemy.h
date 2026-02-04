

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"//让敌人可以继承接口里的函数和类
#include "Enemy.generated.h"

UCLASS()
class SLASH_API AEnemy : public ACharacter,public IHitInterface //继承两个父类
{
	GENERATED_BODY()

public:
	
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	

};
