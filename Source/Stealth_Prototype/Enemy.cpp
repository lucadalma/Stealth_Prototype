#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Stealth_PrototypeCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Stealth_PrototypeGameMode.h"

//Costruttore
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Creazione del PunchCollisionBox
	PunchCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchCollisionBox"));

	// Assicurati che il componente esista prima di lavorarci
	if (PunchCollisionBox)
	{
		// Imposta la scala della collision box
		PunchCollisionBox->SetBoxExtent({ 5.f, 5.f, 5.f }, false);

		// Attacca il componente direttamente allo scheletro nel costruttore
		PunchCollisionBox->SetupAttachment(GetMesh(), TEXT("hand_r_socket"));
	}

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	//Collision boc del pugno aggiuno Begin Overlap e End Overlap

	if (PunchCollisionBox)
	{
		// Solo a runtime possiamo modificare le regole di attacco
		FAttachmentTransformRules const Rules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false
		);

		PunchCollisionBox->AttachToComponent(GetMesh(), Rules, TEXT("hand_r_socket"));
		PunchCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackOverlapBegin);
		PunchCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackOverlapEnd);
	}


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

//Funzione per ottenere il BehaviorTree
UBehaviorTree* AEnemy::GetBehaviorTree() const
{
	return Tree;
}

//Funzione per ottenere il Patrol Path
APatrolPath* AEnemy::GetPatrolPath() const
{
	return PatrolPath;
}

//Finzione per ottenere l'animazione
UAnimMontage* AEnemy::GetMontage() const
{
	return Montage;
}

//Inizio Attacco
void AEnemy::AttackStart()
{
	//First è il profilo di collisione 
	PunchCollisionBox->SetCollisionProfileName("Fist");
	//Il pugno può colpire
	PunchCollisionBox->SetNotifyRigidBodyCollision(true);
}
//Fine Attacco
void AEnemy::AttackEnd()
{
	PunchCollisionBox->SetCollisionProfileName("Fist");
	//Il pugno non può colpire
	PunchCollisionBox->SetNotifyRigidBodyCollision(false);
}
//Funzione per l'animazione di attacco
int AEnemy::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}

	return 0;
}
//Overlap Begin del pugno con il plauyer
void AEnemy::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult)
{
	//Chech per non colpirsi da solo
	if (OtherActor == this)
	{
		return;
	}
	//Ottengo il player
	if (auto const Player = Cast<AStealth_PrototypeCharacter>(OtherActor))
	{
		//Health del player dopo essere colpito
		int const NewHealth = Player->GetHealth() - 5;
		//Setto la vita del player
		Player->SetHealth(NewHealth);
		//Controllo se il player è morto
		if (Player->IsDead())
		{
			//Ottengo la GameMode
			AStealth_PrototypeGameMode* GameMode = GetWorld()->GetAuthGameMode<AStealth_PrototypeGameMode>();

			if (GameMode != nullptr)
			{
				//Chiamo la funzione PawnKilled passandogli il player
				GameMode->PawnKilled(Player);
				//Animazione di morte del player
				/*if (Player->DeathMontage)
				{
					UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
					if (AnimInstance)
					{
						AnimInstance->Montage_Play(Player->DeathMontage);
					}
				}*/
				//Disabilito le collisioni e attivo il ragdoll
				Player->EnableRagdoll();
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				DetachFromControllerPendingDestroy();

			}

		}
	}
}

//Overlap End
void AEnemy::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* OtherComponent, int const OtherBodyIndex)
{

}

//Funzione per l'animazione di morte del player
void AEnemy::PlayDeathAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		//Esegui animazione
		AnimInstance->Montage_Play(DeathMontage);

		// Timer per attivare il ragdoll dopo l'animazione
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemy::EnableRagdoll, 3.f, false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	else
	{
		//Se non c'è animazione, attiva subito il ragdoll
		EnableRagdoll();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//Attiva il Ragdoll del nemico
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