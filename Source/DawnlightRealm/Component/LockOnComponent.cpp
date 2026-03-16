#include "LockOnComponent.h"
#include "../PlayerCharacterBased.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/OverlapResult.h"
#include "../Enemy/EnemyBased.h"



// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!LockedTarget) return;

    
    APlayerCharacterBased* Player =
        Cast<APlayerCharacterBased>(GetOwner());

    if (!Player) return;

    // 攻击 / 闪避 / 蓄力时不强制转向
    if (Player->bIsAttacking ||
        Player->bIsDodging ||
        Player->bIsCharging ||
        Player->bLockMovement)
    {
        return;
    }

    //  目标死亡 
    if (!IsValid(LockedTarget))
    {
        ClearTarget();
            
        Player->SetLockEnemy(false);
          
        return;
    }


    float Dist = FVector::Dist(
        Player->GetActorLocation(),
        LockedTarget->GetActorLocation()
    );

    // SearchRadius之外自动清除锁敌
    if (Dist > SearchRadius)
    {
        ClearTarget();
        Player->SetLockEnemy(false);
        return;
    }

    // 让角色面向敌人
    FVector Dir =
        LockedTarget->GetActorLocation() -
        Player->GetActorLocation();

    FRotator TargetRot = Dir.Rotation();

    if (Player->GetController())
    {
        Player->GetController()->SetControlRotation(
            FRotator(0, TargetRot.Yaw, 0) // 只旋转左右方向
        );
    }
}

void ULockOnComponent::ToggleLockOn()
{
    APlayerCharacterBased* Player =
        Cast<APlayerCharacterBased>(GetOwner());

    if (!Player) return;

    if (LockedTarget) // 有LockedTarget就解锁
    {
        ClearTarget();
        Player->SetLockEnemy(false);
    }
    else  // 没有LockedTarget就锁定
    {
        FindTarget();

        if (LockedTarget)
        {
            Player->SetLockEnemy(true);
        }
    }
}

void ULockOnComponent::FindTarget()
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return;

    FVector Center = OwnerPawn->GetActorLocation();

    TArray<FOverlapResult> Hits;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);

    GetWorld()->OverlapMultiByChannel(
        Hits,
        Center,
        FQuat::Identity,
        ECC_Pawn,
        Sphere
    );
   
    float BestDist = MAX_FLT;
    AActor* BestTarget = nullptr;
    AEnemyBased* BestEnemy = nullptr;
    for (auto& Hit : Hits) // 最近的敌人
    {
        AActor* Actor = Hit.GetActor();
        if (!Actor || Actor == OwnerPawn) continue;

        if (AEnemyBased* Enemy = Cast<AEnemyBased>(Actor))
        {
            float Dist = FVector::Dist(Center, Actor->GetActorLocation());
            if (Dist < BestDist)
            {
                BestDist = Dist;
                BestTarget = Actor;
                BestEnemy = Enemy;
                
            }
        }
    }
    if (!BestEnemy) return;
    BestEnemy->ShowLockOnUI();
    LockedTarget = BestTarget;
    
}



void ULockOnComponent::ClearTarget()
{
    AEnemyBased* Enemy = Cast<AEnemyBased>(LockedTarget);
    if (Enemy)
    {
        Enemy->HideLockOnUI();
    }
    LockedTarget = nullptr;
}

