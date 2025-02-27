// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Stealth_PrototypeCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PunchCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchCollisionBox"));

	if (PunchCollisionBox)
	{
		PunchCollisionBox->SetBoxExtent({ 5.f,5.f,5.f }, false);
		FAttachmentTransformRules const Rules{
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false };

		PunchCollisionBox->AttachToComponent(GetMesh(), Rules, "hand_r_socket");
		PunchCollisionBox->SetRelativeLocation({ -7.f, 0.f, 0.f });

	}

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	PunchCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackOverlapBegin);
	PunchCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackOverlapEnd);


}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AEnemy::GetBehaviorTree() const
{
	return Tree;
}

APatrolPath* AEnemy::GetPatrolPath() const
{
	return PatrolPath;
}

UAnimMontage* AEnemy::GetMontage() const
{
	return Montage;
}

void AEnemy::AttackStart()
{
	PunchCollisionBox->SetCollisionProfileName("Fist");
	PunchCollisionBox->SetNotifyRigidBodyCollision(true);
}

void AEnemy::AttackEnd()
{
	PunchCollisionBox->SetCollisionProfileName("Fist");
	PunchCollisionBox->SetNotifyRigidBodyCollision(false);
}

int AEnemy::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}

	return 0;
}

void AEnemy::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	if (auto const Player = Cast<AStealth_PrototypeCharacter>(OtherActor))
	{
		auto const NewHealth = Player->GetHealth() - 5;
		Player->SetHealth(NewHealth);
	}
}

void AEnemy::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* OtherComponent, int const OtherBodyIndex)
{

}

void AEnemy::PlayDeathAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		// Timer per attivare il ragdoll dopo l'animazione
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemy::EnableRagdoll, 3.f, false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	else
	{
		EnableRagdoll(); // Se non c'è animazione, attiva subito il ragdoll
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemy::EnableRagdoll()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// Se il nemico ha un AI Controller, disabilitalo
	AController* EnemyController = GetController();
	if (EnemyController)
	{
		EnemyController->Destroy();
	}
}

